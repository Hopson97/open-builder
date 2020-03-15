#pragma once

#include "engine_status.h"
#include <atomic>
#include <memory>
#include <optional>
#include <string>
#include <thread>
/*
struct CreateWorldParams {
    std::string name;
    std::string seed;
};

// TEMP
#include "client.h"
#include <server_engine.h>

struct ClientGame {
    std::unique_ptr<Client> client;
    std::unique_ptr<std::thread> serverThread;
    std::unique_ptr<ServerLauncher> serverLauncher;

    EngineStatus createStatus = EngineStatus::Ok;
};

// TEMP END

struct ClientController final {
    enum class ClientState {
        Menu,
        InGame,
        Paused,
    };

    enum class ActionState {
        Idle,
        Working,
        Done,
    };

    class Action {
      public:
        virtual ~Action() = default;

        virtual void apply() = 0;
        virtual ClientGame finish() = 0;
        virtual bool isEmptyAction();
    };

    class NoAction final : public Action {
      public:
        bool isEmptyAction() final override;

        void apply() final override;
        ClientGame finish() final override;
    };

    class CreateWorldAction final : public Action {
      public:
        CreateWorldAction();

        void apply() final override;
        ClientGame finish() final override;

      private:
        ClientGame m_game;
        EngineStatus m_result;
    };

  public:
    ClientController();
    ClientController(ClientController&) = delete;
    ClientController(ClientController&&) = delete;
    ClientController& operator=(ClientController&) = delete;
    ClientController& operator=(ClientController&&) = delete;
    ~ClientController();

    void createWorld();
    void loadWorld(const std::string& fileName);
    void connectToServer(const std::string& ipAddress);

    void applyAction();
    bool isActionComplete();
    [[nodiscard]] ClientGame completeAction();

  private:
    void resetAction();

    std::unique_ptr<Action> m_currentAction;
    std::unique_ptr<std::thread> m_actionApplyThread;

    ClientState m_clientState = ClientState::Menu;
    std::atomic<ActionState> m_actionState = ActionState::Idle;
};
*/