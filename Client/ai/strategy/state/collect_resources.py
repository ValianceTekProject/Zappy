##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## CollectResourcesState - Collecte intelligente des ressources pour incantation
##

import time
from typing import Optional, Any, Dict, List
from ai.strategy.fsm import State, Event
from ai.strategy.pathfinding import Pathfinder
from config import Constants, CommandType
from utils.logger import logger

class CollectResourcesState(State):
    """
    État de collecte de ressources (pierres) pour l'incantation.
    
    Objectifs:
    1. Collecter les ressources manquantes pour le niveau actuel
    2. Prioriser selon les besoins d'incantation
    3. Gérer la nourriture en parallèle (seuil de sécurité)
    4. Optimiser les trajets de collecte
    """
    
    def __init__(self, planner):
        super().__init__(planner)
        self.pathfinder = Pathfinder()
        
        # État de collecte de ressources
        self.resource_target = None
        self.movement_commands = []
        self.current_priority_resource = None
        
        # Tentatives et optimisation
        self.collection_attempts = 0
        self.max_collection_attempts = 3
        self.failed_resources = set()  # Ressources temporairement inaccessibles
        
        # Timing
        self.last_inventory_check = time.time()
        self.inventory_check_interval = 12.0  # Moins fréquent qu'en collecte de nourriture
        
        # Statistiques
        self.resources_collected = {}
        self.collection_session_start = time.time()
        
        logger.info(f"[CollectResourcesState] ⚒️ Collecte ressources niveau {self.state.level} activée")

    def execute(self) -> Optional[Any]:
        """
        Logique de collecte de ressources optimisée.
        Balance entre progression et sécurité alimentaire.
        """
        current_time = time.time()
        
        # 1. Vérification sécurité alimentaire (priorité sur ressources)
        current_food = self.state.get_food_count()
        if current_food <= self._get_food_safety_threshold():
            logger.warning(f"[CollectResourcesState] Nourriture insuffisante pour collecte ({current_food}), transition")
            return None  # Le planner gérera la transition vers collecte nourriture
        
        # 2. Vérification inventaire périodique
        if self._should_check_inventory(current_time):
            logger.debug("[CollectResourcesState] Vérification inventaire périodique")
            self.last_inventory_check = current_time
            return self.cmd_mgr.inventory()
        
        # 3. Mise à jour vision si nécessaire
        if self._needs_vision_update():
            logger.debug("[CollectResourcesState] Mise à jour vision nécessaire")
            self.context['needs_vision_update'] = False
            return self.cmd_mgr.look()
        
        # 4. Ramasser ressource sur tuile actuelle
        current_tile_resource = self._get_needed_resource_on_tile()
        if current_tile_resource:
            logger.info(f"[CollectResourcesState] ⚒️ {current_tile_resource} disponible ici, ramassage")
            return self.cmd_mgr.take(current_tile_resource)
        
        # 5. Exécuter commandes de déplacement en queue
        if self.movement_commands:
            next_cmd = self.movement_commands.pop(0)
            logger.debug(f"[CollectResourcesState] Déplacement vers {self.current_priority_resource}: {next_cmd}")
            return self._execute_movement_command(next_cmd)
        
        # 6. Chercher et cibler nouvelle ressource prioritaire
        priority_target = self._find_priority_resource_target()
        if priority_target:
            if priority_target != self.resource_target:
                self.resource_target = priority_target
                self.current_priority_resource = priority_target.resource_type
                self.movement_commands = self._plan_resource_collection_path(priority_target)
                distance = abs(priority_target.rel_position[0]) + abs(priority_target.rel_position[1])
                logger.info(f"[CollectResourcesState] 🎯 Cible {priority_target.resource_type} à distance {distance}")
            
            if self.movement_commands:
                next_cmd = self.movement_commands.pop(0)
                return self._execute_movement_command(next_cmd)
        
        # 7. Aucune ressource visible - exploration ou vérification complétion
        if self._all_resources_collected():
            logger.info("[CollectResourcesState] ✅ Toutes les ressources collectées!")
            return None  # Transition vers état suivant
        
        return self._explore_for_resources()

    def _get_food_safety_threshold(self) -> int:
        """Seuil de sécurité alimentaire pour continuer la collecte."""
        base = 25  # Plus élevé que le seuil normal de collecte nourriture
        if self.state.level >= 7:
            return int(base * 1.8)
        elif self.state.level >= 4:
            return int(base * 1.4)
        return base

    def _should_check_inventory(self, current_time: float) -> bool:
        """Détermine si un check d'inventaire est nécessaire."""
        if self.context.get('needs_inventory_check', False):
            self.context['needs_inventory_check'] = False
            return True
        
        time_since_last = current_time - self.last_inventory_check
        return time_since_last >= self.inventory_check_interval

    def _needs_vision_update(self) -> bool:
        """Détermine si une mise à jour de vision est nécessaire."""
        if self.context.get('needs_vision_update', False):
            return True
        
        vision = self.state.get_vision()
        if not vision.last_vision_data:
            return True
        
        if getattr(self.state, 'needs_look', False):
            return True
        
        return False

    def _get_needed_resource_on_tile(self) -> Optional[str]:
        """Trouve une ressource nécessaire présente sur la tuile actuelle."""
        vision = self.state.get_vision()
        needed_resources = self._get_missing_resources()
        
        for data in vision.last_vision_data:
            if data.rel_pos == (0, 0):
                for resource in needed_resources:
                    if resource in data.resources and data.resources[resource] > 0:
                        return resource
        return None

    def _get_missing_resources(self) -> Dict[str, int]:
        """Retourne les ressources manquantes pour l'incantation actuelle."""
        requirements = self.state.get_incantation_requirements()
        inventory = self.state.get_inventory()
        
        missing = {}
        for resource, needed in requirements.items():
            current = inventory.get(resource, 0)
            if current < needed:
                missing[resource] = needed - current
        
        return missing

    def _find_priority_resource_target(self):
        """Trouve la ressource prioritaire la plus proche."""
        missing_resources = self._get_missing_resources()
        if not missing_resources:
            return None
        
        vision = self.state.get_vision()
        visible_resources = vision.get_visible_resources()
        
        # Priorité selon la rareté et les besoins
        priority_order = self._get_resource_priority_order(missing_resources)
        
        for resource in priority_order:
            if resource in self.failed_resources:
                continue  # Skip ressources temporairement inaccessibles
                
            positions = visible_resources.get(resource, [])
            valid_positions = [pos for pos in positions if pos != (0, 0)]
            
            if valid_positions:
                # Prendre la plus proche
                closest_pos = min(valid_positions, key=lambda pos: abs(pos[0]) + abs(pos[1]))
                
                class ResourceTarget:
                    def __init__(self, pos, res_type):
                        self.rel_position = pos
                        self.resource_type = res_type
                
                return ResourceTarget(closest_pos, resource)
        
        return None

    def _get_resource_priority_order(self, missing_resources: Dict[str, int]) -> List[str]:
        """Ordonne les ressources par priorité de collecte."""
        # Ordre basé sur la rareté dans le jeu (du plus rare au plus commun)
        rarity_order = [
            Constants.THYSTAME.value,    # 0.05 density
            Constants.PHIRAS.value,      # 0.08 density  
            Constants.MENDIANE.value,    # 0.1 density
            Constants.SIBUR.value,       # 0.1 density
            Constants.DERAUMERE.value,   # 0.15 density
            Constants.LINEMATE.value     # 0.3 density
        ]
        
        # Filtrer seulement les ressources manquantes et ordonner par rareté
        priority_list = [res for res in rarity_order if res in missing_resources]
        
        logger.debug(f"[CollectResourcesState] Priorité ressources: {priority_list}")
        return priority_list

    def _plan_resource_collection_path(self, target):
        """Planifie le chemin optimal vers la ressource."""
        vision_data = self.state.get_vision().last_vision_data
        if not vision_data:
            return []
        
        commands = self.pathfinder.get_commands_to_target(
            target,
            self.state.get_orientation(),
            vision_data
        )
        
        # Limiter le trajet (ressources moins urgentes que nourriture)
        max_commands = 10
        return commands[:max_commands] if commands else []

    def _execute_movement_command(self, command_type: CommandType):
        """Exécute une commande de mouvement."""
        if command_type == CommandType.FORWARD:
            return self.cmd_mgr.forward()
        elif command_type == CommandType.LEFT:
            return self.cmd_mgr.left()
        elif command_type == CommandType.RIGHT:
            return self.cmd_mgr.right()
        else:
            logger.warning(f"[CollectResourcesState] Commande inconnue: {command_type}")
            return None

    def _explore_for_resources(self):
        """Exploration pour trouver des ressources."""
        vision_data = self.state.get_vision().last_vision_data
        
        if not vision_data:
            return self.cmd_mgr.look()
        
        exploration_cmd = self.pathfinder.get_exploration_direction(
            self.state.get_orientation(),
            vision_data
        )
        
        logger.debug(f"[CollectResourcesState] 🔍 Exploration pour ressources: {exploration_cmd}")
        return self._execute_movement_command(exploration_cmd)

    def _all_resources_collected(self) -> bool:
        """Vérifie si toutes les ressources nécessaires sont collectées."""
        missing = self._get_missing_resources()
        is_complete = len(missing) == 0
        
        if is_complete:
            session_time = time.time() - self.collection_session_start
            logger.info(f"[CollectResourcesState] ✅ Collection complète en {session_time:.1f}s: {self.resources_collected}")
        
        return is_complete

    def on_command_success(self, command_type, response=None):
        """Gestion du succès des commandes."""
        if command_type == CommandType.TAKE:
            resource = self.current_priority_resource
            if resource:
                # Statistiques
                self.resources_collected[resource] = self.resources_collected.get(resource, 0) + 1
                logger.info(f"[CollectResourcesState] ✅ {resource} collecté! Total: {self.resources_collected}")
                
                # Mise à jour vision sans LOOK
                vision = self.state.get_vision()
                vision.remove_resource_at((0, 0), resource)
                
                # Reset cible et tentatives
                self.resource_target = None
                self.movement_commands.clear()
                self.collection_attempts = 0
                self.current_priority_resource = None
                
                # Retirer de la liste des échecs si présent
                self.failed_resources.discard(resource)
        
        elif command_type in [CommandType.FORWARD, CommandType.LEFT, CommandType.RIGHT]:
            # Programmer mise à jour vision après mouvement
            self.context['needs_vision_update'] = True
        
        elif command_type == CommandType.INVENTORY:
            self.last_inventory_check = time.time()

    def on_command_failed(self, command_type, response=None):
        """Gestion des échecs de commandes."""
        if command_type == CommandType.TAKE:
            self.collection_attempts += 1
            resource = self.current_priority_resource
            
            logger.warning(f"[CollectResourcesState] ❌ Échec collecte {resource}, tentative {self.collection_attempts}")
            
            # Marquer comme temporairement inaccessible
            if resource and self.collection_attempts >= 2:
                self.failed_resources.add(resource)
                logger.warning(f"[CollectResourcesState] {resource} marquée comme inaccessible temporairement")
            
            # Reset cible
            self.resource_target = None
            self.movement_commands.clear()
            self.context['needs_vision_update'] = True
        
        elif command_type in [CommandType.FORWARD, CommandType.LEFT, CommandType.RIGHT]:
            stuck_counter = self.context.get('stuck_counter', 0) + 1
            self.context['stuck_counter'] = stuck_counter
            
            if stuck_counter >= 2:
                logger.warning("[CollectResourcesState] Mouvements bloqués, reset cible")
                self.resource_target = None
                self.movement_commands.clear()

    def on_event(self, event: Event) -> Optional[State]:
        """Gestion des événements."""
        if event == Event.FOOD_EMERGENCY:
            logger.warning("[CollectResourcesState] Urgence alimentaire, arrêt collecte ressources")
            from ai.strategy.state.emergency import EmergencyState
            return EmergencyState(self.planner)
        
        elif event == Event.FOOD_LOW:
            current_food = self.state.get_food_count()
            threshold = self._get_food_safety_threshold()
            
            if current_food <= threshold:
                logger.info(f"[CollectResourcesState] Nourriture faible ({current_food} <= {threshold}), collecte nourriture")
                from ai.strategy.state.collect_food import CollectFoodState
                return CollectFoodState(self.planner)
        
        elif event == Event.RESOURCES_COLLECTED:
            if self._all_resources_collected():
                logger.info("[CollectResourcesState] ✅ Ressources complètes, transition exploration")
                from ai.strategy.state.explore import ExploreState
                return ExploreState(self.planner)
        
        return None

    def on_enter(self):
        """Actions à l'entrée de l'état."""
        super().on_enter()
        missing = self._get_missing_resources()
        logger.info(f"[CollectResourcesState] ⚒️ ENTRÉE collecte ressources - Manquants: {missing}")
        
        # Reset état interne
        self.resource_target = None
        self.movement_commands.clear()
        self.collection_attempts = 0
        self.failed_resources.clear()
        self.resources_collected.clear()
        self.collection_session_start = time.time()
        
        # Forcer mise à jour vision
        self.context['needs_vision_update'] = True

    def on_exit(self):
        """Actions à la sortie de l'état."""
        super().on_exit()
        session_time = time.time() - self.collection_session_start
        logger.info(f"[CollectResourcesState] ✅ SORTIE collecte ressources - Durée: {session_time:.1f}s, Collecté: {self.resources_collected}")
        
        # Nettoyage
        self.resource_target = None
        self.movement_commands.clear()
        self.failed_resources.clear()