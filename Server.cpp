
#include <iostream>
#include <unistd.h>
#include <sys/prctl.h>
//#include <linux/seccomp.h>
#include <seccomp.h>
#include <linux/capability.h>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio/signal_set.hpp>
#include "Logger.hpp"
#include "TCP_Server.hpp"

typedef std::unique_ptr<std::mutex> mutex_ptr;
bool init_seccomp();

std::string get_cwd();
int main(int argc, char* argv[])
{
	
	if (argc != 2) 
	{
		std::cout << "Error, No port Number required" << std::endl;
		
		return 1;
	}
	

	//prctl(PR_SET_NO_NEW_PRIVS, 1); // is inherited by child threads (fork/clone)
	//prctl(PR_SET_DUMPABLE, 0);
	//for(int i = 0; i < filter_list.size(); ++i){
	//	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(c.str(filter_list[i])), 1);
	//}
	

	bool seccomp_ = init_seccomp();



		std::string cwd = get_cwd();  // get current directory
    	int success = chroot(cwd.c_str()); // chroots the directory the server is started in
    	if(chroot(cwd.c_str()))
    	{
        	std::cout << "CHROOT success\n";
        	cwd = get_cwd();
        	std::cout <<"Current directory after CHROOT " << cwd <<std::endl; //debug
   		}
    
   		else
    	{

        	std::cout << "CHROOT Unsuccessfull, please check permissions\n";
   		}
		boost::asio::io_context context;
		boost::asio::signal_set signal(context, SIGINT, SIGTERM);
		mutex_ptr access_log_mutex;
		mutex_ptr error_log_mutex;
		Logger log(access_log_mutex);
		TCP_Server server(context, std::atoi(argv[1]), log); // standard server start with IO, and port number
	//}
	//else
	//{
	//	std::cout << "Seccomp failed\n";
	//	return 1;
	//}
}

std::string get_cwd()
{
    char buff[128];
    getcwd(buff,128);
    std::string dir(buff);
    return dir;

}

bool init_seccomp()
{
 
 	scmp_filter_ctx ctx;
 	prctl(PR_SET_NO_NEW_PRIVS, 1); // is inherited by child threads (fork/clone)
	prctl(PR_SET_DUMPABLE, 0);
 	ctx = seccomp_init(SCMP_ACT_NOTIFY);


	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(accept), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(listen), 0);
 
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(bind), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(setsockopt), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(epoll_ctl), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(socket), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(stat), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(lseek), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(openat), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(rt_sigaction), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(fcntl), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(pipe), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(timerfd_create), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(epoll_create1), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(eventfd2), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(chroot), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getcwd), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(fstat), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(futex), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(brk), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(prlimit64), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(rt_sigprocmask), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(set_robust_list), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(set_tid_address), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(munmap), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(arch_prctl), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(openat), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(pread64), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(access), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mmap), 0);

	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(clone), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(ptrace), 0);




	seccomp_load(ctx);


// corresponding sig call numbers
// https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#x86_64-64_bit
//43,50,49,54,233,41,1,4,8,257,13,72,22,283,291,
//290,161,79,5,202,12,302,14,273,218,11,10,158,
//0,17,10,21,59,9 


return true;
}
