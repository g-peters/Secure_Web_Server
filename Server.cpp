
#include <iostream>
#include <unistd.h>
#include <sys/prctl.h>
#include <seccomp.h>
#include <linux/capability.h>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio/signal_set.hpp>
#include "Logger.hpp"
#include "TCP_Server.hpp"

typedef std::unique_ptr<std::mutex> mutex_ptr;
// function delcarations
bool init_seccomp();
std::string get_cwd();


int main(int argc, char* argv[])
{
	
	if (argc != 2) 
	{
		std::cout << "Error, No port Number required" << std::endl;
		return 1;
	}
	// prctl used to set linux capabilities, code adapted from 

	prctl(PR_SET_NO_NEW_PRIVS, 1); 
	prctl(PR_SET_DUMPABLE, 0);
	boost::asio::io_context io;
	mutex_ptr access_log_mutex;
	mutex_ptr error_log_mutex;

	// get current directory
	std::string cwd = get_cwd();  
	// chroots the directory the server is started in, code adapted from (Kerrisk, 2020a)
    int success = chroot(cwd.c_str()); 
    if(success == 0)
    {
        cwd = get_cwd();
   	}
   	else
    {
        std::cout << "CHROOT Unsuccessfull, please check permissions\n";
        return 1;
   	}
   	// lower permissions from root to cyber
   	setreuid(1000,1000); 
   	// intilialize seccomp rules, this also removes the ability to set setruid
	bool seccomp_ = init_seccomp(); 
	// creates logger objects
	Logger access_log(access_log_mutex,"access_log.txt"); 
	Logger error_log(error_log_mutex,"error_log.txt");
    //standard server start with IO, and port number
	TCP_Server server(io, std::atoi(argv[1]), access_log, error_log);

	
}
// gets current working directory to be used to set chroot
std::string get_cwd()
{
    char buff[128];
    getcwd(buff,128);
    std::string dir(buff);
    return dir;

}

// intitialize seccomp rules, the minimum rules required for the server to run
// code adapted from (Kerrisk, 2020b)
bool init_seccomp()
{
 
 	scmp_filter_ctx ctx;
 	// SCMP_ACT_KILL, if system call from below list is called, seccomp will kill the process
 	// sycalls made was determined using strace of the program and recording each as it was made
 	ctx = seccomp_init(SCMP_ACT_KILL);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(recvmsg), 0);
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
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(close), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(sendmsg), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mkdir), 0);

	seccomp_load(ctx);


return true;
}



// References
/*

boost. (n.d.). doc/html/boost_asio/example/cpp11/http/server/connection.cpp. Boost.org. 
	Retrieved 2021, from https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/example/cpp11/http/server/connection.cpp

boost. (2017). Filesystem Tutorial. Boost.org. https://www.boost.org/doc/libs/1_75_0/libs/filesystem/doc/tutorial.html

cppreference.com. (2017). std::basic_ofstream. Cppreference.Com. https://en.cppreference.com/w/cpp/io/basic_ofstream

cppreference.com. (2020). std::for_each. Cppreference.Com. https://en.cppreference.com/w/cpp/algorithm/for_each

cppreference.com. (2021). Lambda expressions. Cppreference.Com. https://en.cppreference.com/w/cpp/language/lambda

Drodil. (2018). drodil/cpp-util. GitHub. https://github.com/drodil/cpp-util/blob/master/file/mime/detector.hpp

Kerrisk, M. (2020a). chroot(2) - Linux manual page. Man7.Org. https://man7.org/linux/man-pages/man2/chroot.2.html

Kerrisk, M. (2020b). seccomp(2) - Linux manual page. Man7.Org. https://man7.org/linux/man-pages/man2/seccomp.2.html
*/