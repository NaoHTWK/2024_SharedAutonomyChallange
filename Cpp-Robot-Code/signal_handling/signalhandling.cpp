#include "signalhandling.h"

#include <atomic>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>

#include <signal.h>
#include <unistd.h>

#include <boost/filesystem.hpp>
#include <algorithm_ext.h>

namespace bf = boost::filesystem;

namespace SignalHandling {

std::atomic_bool shutdown_flag(false);
static_assert(std::atomic_bool::is_always_lock_free);
uint32_t wdg_timeout = 5'000'000;

static void my_signal_handler(int /*signum*/)
{
    // ok, lock-free atomics
    shutdown_flag = true;

    // ok, write is signal-safe
    const char str[] = "\033[31mtermination signal received!\033[0m\n";
    write(STDERR_FILENO, str, sizeof(str) - 1);

    // Restore default signal handler so if we did something wrong the process can be killed.
    //signal(SIGINT, SIG_DFL);
}

static void watchdog() {
    while(!shutdown())
        usleep(0.5_s);

    usleep(wdg_timeout);
    printf("\033[31m%s: Woof woof grrrrrrrr!!!11!!\033[0m\n", __PRETTY_FUNCTION__);
    fflush(stdout);
    _exit(0);
}

static void kill_me_file_watcher() {
    bf::path kill_me_file("/tmp/kill_me");


    // If we start and the file already exists make sure we have to chance to start.
    if(bf::exists(kill_me_file))
        bf::remove(kill_me_file);

    while(!shutdown()) {
        usleep(0.5_s);

        if(bf::exists(kill_me_file)) {
            printf("\033[31mReset file found: Request shutdown!\033[0m\n");
            requestShutdown();
        }
    }
}

void init() {
    struct sigaction action;
    action.sa_handler = my_signal_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if(sigaction(SIGINT, &action, NULL)) {
        printf("Error installing signal handler! %s: %s\n", __PRETTY_FUNCTION__, strerror(errno));
        exit(1);
    }

    if(sigaction(SIGTERM, &action, NULL)) {
        printf("Error installing signal handler! %s: %s\n", __PRETTY_FUNCTION__, strerror(errno));
        exit(1);
    }

    std::thread watchdog_thread(watchdog);
    watchdog_thread.detach();

    std::thread killme_file_watcher_thread(kill_me_file_watcher);
    killme_file_watcher_thread.detach();
}

bool shutdown() {
    return shutdown_flag.load(std::memory_order::memory_order_relaxed);
}

void requestShutdown() {
    printf("\033[31mShutdown was requested!\033[0m\n");
    fflush(stdout);
    shutdown_flag = true;
}

void setWatchdogTimeout(uint32_t timeout_in_us) {
    wdg_timeout = timeout_in_us;
}

}

