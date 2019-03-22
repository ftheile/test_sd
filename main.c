#include <systemd/sd-journal.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>

static bool cont = true;

static void handler(int sig, siginfo_t* si, void* unused)
{
	sd_journal_print(LOG_NOTICE, "in handler()");
	cont = false;
}

int main(int argc, char *argv[])
{
	struct sigaction sa;

	sd_journal_print(LOG_NOTICE, "initializing");
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;
	if (sigaction(SIGTERM, &sa, NULL) != -1) {
		do {
			sd_journal_print(LOG_NOTICE, "running...");
			sleep(10);
		} while (cont);
		sd_journal_print(LOG_NOTICE, "finished");
	} else {
		sd_journal_print(LOG_NOTICE, "initialization failed!");
	}
	return 0;
}
