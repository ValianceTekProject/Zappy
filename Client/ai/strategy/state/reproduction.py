##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## ReproductionState - Gestion de la reproduction après niveau 2 - CORRIGÉ
##

import time
from typing import Optional, Any
from ai.strategy.fsm import State, Event
from config import Constants, CommandType
from utils.logger import logger

class ReproductionState(State):
    """
    État de reproduction - Création d'un nouvel agent après niveau 2.
    
    Objectifs:
    1. Vérifier les conditions de sécurité pour la reproduction
    2. Exécuter la séquence connect_nbr -> fork (si nécessaire) -> créer agent
    3. Gérer la logique de l'ancien planner pour les enfants
    4. Transition vers exploration après succès
    """
    
    def __init__(self, planner):
        super().__init__(planner)
        
        # Phases de reproduction (logique de l'ancien planner)
        self.fork_stage = 0  # 0=init, 1=connect_nbr, 2=fork, 3=créer_agent, 4=terminé
        self.new_agent_created = False  # Flag pour savoir si l'agent a été créé
        self.should_create_agent = False  # Flag pour déclencher création
        
        # Seuils de sécurité
        self.min_food_for_fork = self._calculate_min_food_for_fork()
        self.reproduction_start_time = time.time()
        self.reproduction_timeout = 30.0  # 30 secondes max
        
        # Statistiques
        self.fork_attempts = 0
        self.max_fork_attempts = 2
        
        logger.info(f"[ReproductionState] 👶 Préparation reproduction - Food: {self.state.get_food_count()}")

    def execute(self) -> Optional[Any]:
        """
        Logique de reproduction en phases sécurisées.
        Utilise la logique éprouvée de l'ancien planner.
        """
        current_time = time.time()
        
        # 1. Vérification sécurité alimentaire
        current_food = self.state.get_food_count()
        if current_food < self.min_food_for_fork:
            logger.warning(f"[ReproductionState] Nourriture insuffisante pour fork ({current_food} < {self.min_food_for_fork})")
            return None  # Retour à la collecte de nourriture
        
        # 2. Timeout de sécurité
        if current_time - self.reproduction_start_time > self.reproduction_timeout:
            logger.warning("[ReproductionState] Timeout reproduction, abandon")
            return None
        
        # 3. ✅ NOUVEAU: Vérifier si on doit créer un agent
        if self.should_create_agent and not self.new_agent_created:
            self._create_new_agent()
            return None
        
        # 4. Gestion phases de fork (logique originale)
        return self._handle_fork_sequence()

    def _create_new_agent(self):
        """✅ NOUVEAU: Crée effectivement le nouvel agent."""
        logger.info("[ReproductionState] 🎉 CRÉATION NOUVEL AGENT")
        
        # ✅ CORRECTION: Vérifier plusieurs emplacements possibles pour agent_thread
        agent_thread = None
        
        # Méthode 1: Via self.state.agent_thread
        if hasattr(self.state, 'agent_thread') and self.state.agent_thread is not None:
            agent_thread = self.state.agent_thread
            logger.debug("[ReproductionState] agent_thread trouvé dans self.state")
        
        # Méthode 2: Via self.planner.agent_thread (fallback)
        elif hasattr(self.planner, 'agent_thread') and self.planner.agent_thread is not None:
            agent_thread = self.planner.agent_thread
            logger.debug("[ReproductionState] agent_thread trouvé dans self.planner")
        
        if agent_thread is not None:
            agent_thread.create_new_agent()
            logger.info("[ReproductionState] ✅ Nouvel agent créé via agent_threads")
        else:
            logger.error("[ReproductionState] ❌ Impossible de créer nouvel agent: agent_thread introuvable")
            logger.error(f"[ReproductionState] Debug state: hasattr={hasattr(self.state, 'agent_thread')}, value={getattr(self.state, 'agent_thread', 'MISSING')}")
            logger.error(f"[ReproductionState] Debug planner: hasattr={hasattr(self.planner, 'agent_thread')}, value={getattr(self.planner, 'agent_thread', 'MISSING')}")
        
        # Marquer comme créé pour éviter de recréer
        self.new_agent_created = True
        self.should_create_agent = False
        self.fork_stage = 4  # Terminé
        
        # Programmer une vision pour mettre à jour l'état
        self.context['needs_vision_update'] = True

    def _calculate_min_food_for_fork(self) -> int:
        """Calcule la nourriture minimale pour un fork sécurisé."""
        # Estimation coût fork + marge sécurité pour survie parent et enfant
        base_cost = 3  # Coût estimé du fork
        safety_margin = 10  # Marge pour survie
        
        if self.state.level >= 3:
            return int((base_cost + safety_margin) * 1.5)
        return base_cost + safety_margin

    def _handle_fork_sequence(self) -> Optional[Any]:
        """
        Gère la séquence de fork - LOGIQUE CORRIGÉE.
        """
        if self.fork_stage == 0:
            logger.info("[ReproductionState] Phase 0: Vérification slots disponibles")
            self.fork_stage = 1
            return self.cmd_mgr.connect_nbr()

        elif self.fork_stage == 1:
            logger.info("[ReproductionState] Phase 1: Analyse réponse connect_nbr")
            
            # Récupérer la dernière réponse connect_nbr
            last_connect = self.cmd_mgr.get_last_success(CommandType.CONNECT_NBR)
            if not last_connect:
                logger.debug("[ReproductionState] Attente réponse connect_nbr...")
                return None

            slots = int(last_connect.response)
            logger.info(f"[ReproductionState] Slots disponibles: {slots}")
            
            if slots == 0 and self._has_enough_for_fork():
                logger.info("[ReproductionState] Aucun slot libre et assez de ressources → FORK!")
                self.fork_stage = 2
                return self.cmd_mgr.fork()
            elif slots > 0:
                # ✅ CORRECTION: Si on a des slots, créer directement l'agent
                logger.info(f"[ReproductionState] {slots} slots disponibles → Création directe d'agent")
                self.should_create_agent = True
                self.fork_stage = 3  # Aller à la phase de création
                return None
            else:
                logger.info(f"[ReproductionState] Conditions non réunies (slots={slots}, ressources={self._has_enough_for_fork()})")
                self.fork_stage = 4  # Terminé sans succès
                return None

        elif self.fork_stage == 2:
            logger.info("[ReproductionState] Phase 2: Attente résultat fork")
            # Le fork est en cours, on attend la réponse
            # La transition se fera via on_command_success/failed
            return None

        elif self.fork_stage == 3:
            logger.info("[ReproductionState] Phase 3: En attente de création agent")
            # Cette phase est gérée par execute() avec should_create_agent
            return None

        elif self.fork_stage == 4:
            logger.info("[ReproductionState] Phase 4: Reproduction terminée")
            return None  # Transition vers exploration

        return None

    def _has_enough_for_fork(self) -> bool:
        """
        Détermine si on a les ressources minimales pour un fork stratégique.
        LOGIQUE ORIGINALE de l'ancien planner.
        """
        current_food = self.state.get_food_count()
        return current_food >= self.min_food_for_fork

    def on_command_success(self, command_type, response=None):
        """Gestion du succès des commandes."""
        if command_type == CommandType.CONNECT_NBR:
            logger.info(f"[ReproductionState] ✅ Connect_nbr réussi: {response}")
            # La logique continue dans execute()
            
        elif command_type == CommandType.FORK:
            logger.info(f"[ReproductionState] ✅🎉 FORK RÉUSSI! Slot créé")
            # ✅ CORRECTION: Après fork réussi, déclencher création d'agent
            self.should_create_agent = True
            self.fork_stage = 3

    def on_command_failed(self, command_type, response=None):
        """Gestion des échecs de commandes."""
        if command_type == CommandType.CONNECT_NBR:
            logger.warning(f"[ReproductionState] ❌ Connect_nbr échoué: {response}")
            self.fork_attempts += 1
            
            if self.fork_attempts >= self.max_fork_attempts:
                logger.error("[ReproductionState] Trop d'échecs connect_nbr, abandon reproduction")
                self.fork_stage = 4
            else:
                # Retry
                self.fork_stage = 0
                
        elif command_type == CommandType.FORK:
            logger.error(f"[ReproductionState] ❌ FORK ÉCHOUÉ: {response}")
            self.fork_attempts += 1
            
            if self.fork_attempts >= self.max_fork_attempts:
                logger.error("[ReproductionState] Abandon reproduction après échecs")
                self.fork_stage = 4
            else:
                # Retry depuis le début
                self.fork_stage = 0

    def on_event(self, event: Event) -> Optional[State]:
        """Gestion des événements pendant reproduction."""
        if event == Event.FOOD_EMERGENCY:
            logger.error("[ReproductionState] URGENCE ALIMENTAIRE! Abandon reproduction")
            from ai.strategy.state.emergency import EmergencyState
            return EmergencyState(self.planner)
        
        elif event == Event.FOOD_LOW:
            current_food = self.state.get_food_count()
            if current_food <= self.min_food_for_fork:
                logger.warning("[ReproductionState] Nourriture faible, report reproduction")
                from ai.strategy.state.collect_food import CollectFoodState
                return CollectFoodState(self.planner)
        
        return None

    def is_reproduction_complete(self) -> bool:
        """Vérifie si la reproduction est terminée."""
        return self.fork_stage == 4 and self.new_agent_created

    def on_enter(self):
        """Actions à l'entrée de l'état."""
        super().on_enter()
        current_food = self.state.get_food_count()
        
        logger.info(f"[ReproductionState] 👶 ENTRÉE reproduction - "
                   f"Niveau: {self.state.level}, Food: {current_food}, "
                   f"Seuil: {self.min_food_for_fork}")
        
        # Reset état interne
        self.fork_stage = 0
        self.new_agent_created = False
        self.should_create_agent = False
        self.fork_attempts = 0
        self.reproduction_start_time = time.time()
        
        # Marquer que la reproduction est demandée (compatibilité)
        self.state.needs_repro = True

    def on_exit(self):
        """Actions à la sortie de l'état."""
        super().on_exit()
        duration = time.time() - self.reproduction_start_time
        
        if self.new_agent_created:
            logger.info(f"[ReproductionState] ✅ SORTIE avec succès - "
                       f"Reproduction terminée en {duration:.1f}s")
        else:
            logger.info(f"[ReproductionState] ❌ SORTIE sans succès - "
                       f"Durée: {duration:.1f}s")
        
        # Marquer la reproduction comme terminée
        self.state.needs_repro = False