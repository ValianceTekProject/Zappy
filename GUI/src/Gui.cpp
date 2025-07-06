/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Gui.cpp
*/

#include "Gui.hpp"

static constexpr const char *defaultIp = "127.0.0.1";

/**
 * @brief Construct a new zappy::gui::Gui::Gui object
 *
 * initialize the gui
 * debug is set to true if the -d or --debug argument is given
 * ip is set to the 127.0.0.1 if no ip is given
 * port is set to 4242 if no port is given
 */
zappy::gui::Gui::Gui() :
    _debug(false),
    _ip(defaultIp),
    _port(4242),
    _protocol(nullptr),
    _gameState(nullptr),
    _renderer(nullptr)
{}

/**
 * @brief Parse the arguments given to the program
 *
 * Parse the arguments given to the program
 *
 * @param argc number of arguments
 * @param argv arguments
 */
void zappy::gui::Gui::parseArgs(int argc, char const *argv[])
{
    if (argc < 3)
        throw ParsingError("Not enough arguments\n\tUsage: ./zappy_gui -p port -h host", "Parsing");

    if (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")) {
        std::cout << "Usage: ./zappy_gui -p port -h host" << std::endl;
        exit(0);
    }

    bool raylib = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-p") {
            if (i + 1 >= argc)
                throw ParsingError("Missing value for -p", "Parsing");
            std::istringstream ss(argv[++i]);
            if (!(ss >> this->_port) || !this->_port)
                throw ParsingError("Invalid port number: " + std::string(argv[i]), "Parsing");
        } else if (arg == "-h") {
            if (i + 1 >= argc)
                throw ParsingError("Missing value for -h", "Parsing");
            this->_ip = argv[++i];
        } else if (arg == "-debug" || arg == "-d") {
            this->_debug = true;
        } else if (arg == "-raylib" || arg == "-r") {
            raylib = true;
        } else
            throw ParsingError("Unknown option: " + arg, "Parsing");
    }

    if (this->_ip.empty())
        throw ParsingError("Host (-h) not specified", "Parsing");
    if (this->_port <= 0 || this->_port > 65535)
        throw ParsingError("Port out of range: " + std::to_string(this->_port), "Parsing");

    if (!raylib && this->_debug) {
        this->_renderer = std::make_shared<DebugRenderer>();
    } else if (!this->_renderer) {
        if (raylib)
            this->_renderer = std::make_shared<RaylibRenderer>();
        else
            this->_renderer = std::make_shared<NcursesRenderer>();
    }
}

/**
 * @brief initialize the game
 *
 * initialize the game state and the renderer
 */
void zappy::gui::Gui::init()
{
    this->_gameState = std::make_shared<game::GameState>();

    this->_renderer->setGameState(this->_gameState);

    _initNetwork();

    while(!_isMapCreated())
        this->_protocol->update();
}

/**
 * @brief initialize the network
 *
 * initialize the network and send the first request to the server
 * initialize the renderer requests to the server
 */
void zappy::gui::Gui::_initNetwork()
{
    this->_protocol = std::make_unique<network::Protocol>(this->_renderer, this->_gameState, this->_debug);
    if (!this->_protocol->connectToServer(this->_ip, this->_port))
        throw network::NetworkError("Connection failed", "Network");

    const ProtocolRequest protocolRequests = {
        { network::GP::MAP_SIZE, [this](const int &, const int &){ this->_protocol->requestMapContent(); } },
        { network::GP::TILE_CONTENT, [this](const int &x, const int &y){ this->_protocol->requestTileContent(x, y); } },
        { network::GP::MAP_CONTENT, [this](const int &, const int &){ this->_protocol->requestMapContent(); } },
        { network::GP::TEAM_NAME, [this](const int &, const int &){ this->_protocol->requestTeamNames(); } },
        { network::GP::PLAYER_POSITION, [this](const int &id, const int &){ this->_protocol->requestPlayerPosition(id); } },
        { network::GP::PLAYER_LEVEL, [this](const int &id, const int &){ this->_protocol->requestPlayerLevel(id); } },
        { network::GP::PLAYER_INVENTORY, [this](const int &id, const int &){ this->_protocol->requestPlayerInventory(id); } },
        { network::GP::TIME_UNIT_REQUEST, [this](const int &, const int &){ this->_protocol->requestTimeUnit(); } },
        { network::GP::TIME_UNIT_MODIFICATION, [this](const int &timeUnit, const int &){ this->_protocol->setTimeUnit(timeUnit); } },
    };

    this->_renderer->setProtocolRequests(protocolRequests);
}

/**
 * @brief Run the GUI
 *
 * initialize the network and the renderer
 * then run the main loop
 */
void zappy::gui::Gui::run()
{
    init();

    bool running = true;
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        this->_renderer->handleInput();

        this->_protocol->update();
        this->_renderer->update();

        if (this->_renderer->shouldClose()) {
            running = false;
            continue;
        }

        if (!this->_protocol->isConnected()) {
            running = false;
            std::cerr << "Connection lost" << std::endl;
            continue;
        }

        this->_renderer->render();
    }

    this->_protocol->disconnect();
}
