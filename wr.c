/* utility functions {{{ */
#ifndef VERSION
#define VERSION "woozy wah"
#endif

#define HCF(fmt,...) fprintf(stderr,"%s:%d:%s():"fmt"\n",__FILE__,__LINE__,__func__,__VA_ARGS__)
#define HCF0(s) fprintf(stderr,"%s:%d:%s():"s"\n",__FILE__,__LINE__,__func__)
#define ERR_MEM "!: insufficient memory"

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;
typedef size_t uz;
typedef int s;
typedef unsigned int u;
typedef char c;
typedef unsigned char uc;
typedef signed char sc;
typedef float f32;
typedef double f64;

#define U64X UINT64_MAX
#define U32X UINT32_MAX
#define U16X UINT16_MAX
#define U8X UINT8_MAX
#define S64X INT64_MAX
#define S64M INT64_MIN
#define S32X INT32_MAX
#define S32M INT32_MIN
#define S16X INT16_MAX
#define S16M INT16_MIN
#define S8X INT8_MAX
#define S8M INT8_MIN
#define ZX SIZE_MAX
#define SX INT_MAX
#define SM INT_MIN
#define UX UINT_MAX
#define z sizeof
#define na restrict
#define fx const
#define st static
#define ex extern
#define fast static inline
#define BITS(a) (z (a)*CHAR_BIT)

fast
s
of_add(u64 a,u64 b,u64 res[st 1])
{
	if(U64X-b>a)
return true;
	*res=a+b;
	return false;
}

fast
s
of_addz(uz a,uz b,uz res[st 1])
{
	if(ZX-b>a)
return true;
	*res=a+b;
	return false;
}

fast
s
of_sub(u64 a,u64 b,u64 res[st 1])
{
	if(a<b)
return true;
	*res=a-b;
	return false;
}

fast
s
of_subz(uz a,uz b,uz res[st 1])
{
	if(a<b)
return true;
	*res=a-b;
	return false;
}

fast
s
of_mul(u64 a,u64 b,u64 res[st 1])
{
	if(!b)
goto zero;
	if(U64X/b>a)
return true;
zero:
	*res=a*b;
	return false;
}

fast
s
of_mulz(uz a,uz b,uz res[st 1])
{
	if(!b)
goto zero;
	if(ZX/b>a)
return true;
zero:
	*res=a*b;
	return false;
}

fast
s
of_div(u64 a,u64 b,u64 res[st 1])
{
	if(!b)
return true;
	*res=a/b;
	return false;
}

fast
s
of_divz(uz a,uz b,uz res[st 1])
{
	if(!b)
return true;
	*res=a/b;
	return false;
}

fast
s
of_rem(u64 a,u64 b,u64 res[st 1])
{
	if(!b)
return true;
	*res=a%b;
	return false;
}

fast
s
of_remz(uz a,uz b,uz res[st 1])
{
	if(!b)
return true;
	*res=a%b;
	return false;
}

fast
s
of_shl(u64 a,u64 b,u64 res[st 1])
{
	if(b>=BITS(b)||a>U64X>>b)
return true;
	*res=a<<b;
	return false;
}

fast
s
of_shlz(uz a,uz b,uz res[st 1])
{
	if(b>=BITS(b)||a>ZX>>b)
return true;
	*res=a<<b;
	return false;
}

fast
s
of_shr(u64 a,u64 b,u64 res[st 1])
{
	if(b>=BITS(b))
return true;
	*res=a>>b;
	return false;
}

fast
s
of_shrz(uz a,uz b,uz res[st 1])
{
	if(b>=BITS(b))
return true;
	*res=a>>b;
	return false;
}

fast
s
of_adds(s64 a,s64 b,s64 res[st 1])
{
	if(a >= 0){
if(b>S64X-a) return true;
	}else{
if(b<S64M-a) return true;
	}
	*res=a+b;
	return false;
}

fast
s
of_subs(s64 a,s64 b,s64 res[st 1])
{
	if(a>=0){
if(b<S64M+a) return true;
	}else{
if(b>S64X+a) return true;
	}
	*res=a-b;
	return false;
}

fast
s
of_muls(s64 a,s64 b,s64 res[st 1])
{
	if(!a||!b)
goto zero;
	if(a>0){
if(b>0){if(b>S64X/a) return true;}else{if(b<S64M/a) return true;}
	}else{
if(b>0){if(a<S64M/b) return true;}else{if(a<S64X/b) return true;}
	}
zero:
	*res=a*b;
	return false;
}

fast
s
of_divs(s64 a,s64 b,s64 res[st 1])
{
	if(!b||(a==S64M&&b==-1))
return true;
	*res=a/b;
	return false;
}

fast
s
of_rems(s64 a,s64 b,s64 res[st 1])
{
	if(!b||(a==S64M&&b==-1))
return true;
	*res=a%b;
	return false;
}

fast
s
of_neg(s64 a,s64 res[st 1])
{
	if(a==S64M)
return true;
	*res=-a;
	return false;
}

fast
s
of_shls(s64 a,s64 b,s64 res[st 1])
{
	if(a<0||b>=(s64)BITS(a)||a>S64X>>b)
return true;
	*res=a<<b;
	return false;
}

fast
s
of_shrs(s64 a,s64 b,s64 res[st 1])
{
	if(a<0||b>=(s64)BITS(a))
return true;
	*res=a>>b;
	return false;
}

/* http://www.isthe.com/chongo/tech/comp/fnv/ */
fast
u64
hash_fnv(fx void *na data,uz sz)
{
	fx uc *p=data;
	u64 hash;
	hash=14695981039346656037ull;
	while(sz--){
		hash^=p[sz];
		hash*=0x100000001b3ull;
	}
	return hash;
}

fast
u64
str_hash_fnv(fx c data[st na 1])
{
	fx uc *p=(fx uc *)data;
	u64 hash;
	hash=14695981039346656037ull;
	while(*p){
		hash^=*p++;
		hash*=0x100000001b3ull;
	}
	return hash;
}

/* safely convert double to unsigned integer */
/* returns 0 in the case of negativity or NaN */
fast
u64
dconv(f64 a)
{
	s exp;
	if(isnan(a)||a<0.0)
return 0;
	frexp(a,&exp);
	if(isfinite(a)&&exp<=(s)BITS(u64)-1)
return a;
	return U64X;
}

/* safely convert double to signed integer */
/* returns 0 in the case of NaN */
fast
s64
dconvs(f64 a)
{
	s exp;
	if(isnan(a))
return 0;
	frexp(a,&exp);
	if(isfinite(a)&&exp<=(s)BITS(s64)-2)
return a;
	return signbit(a)?S64M:S64X;
}

/* return number of utf8 code points in given string */
fast
uz
utf8len(fx c xs[st 1])
{
	uz ret;

	ret=0;
	while(*xs) if((*xs++ & 0xc0)!=0x80) ret++;
	return ret;
}

/* clip utf8 string to n or less code points */
fast
void
utf8clip(c xs[st 1],uz n)
{
	while(n&&*xs) if((*xs++&0xc0)!=0x80) n--;
	while(*xs&&(*xs++&0xc0)==0x80);
	*xs=0;
}

/* strstr, except case-insensitive */
fast
fx c *
xistrstr(fx c xs[st 1],fx c h[st 1])
{
	fx c *tmp_s;
	fx c *tmp_h;

	for(;*xs;xs++){
		for(;*xs&&toupper((uc)*xs)!=toupper((uc)*h);xs++);
		if(!*xs)
return 0;
		tmp_s=xs;
		tmp_h=h;
		while(toupper((uc)*tmp_s++)==toupper((uc)*tmp_h++)) if (!*tmp_h) return xs;
	}
	return 0;
}

fast
void
strrep(c xs[st 1],s ch,s r)
{
	for(;*xs;xs++) if(*xs==ch) *xs=r;
}

fast
void
strrepa(c xs[st 1],s (*cr)[2],uz len)
{
	uz i;
	for(;*xs;xs++) for(i=0;i<len;i++) if(*xs==cr[i][0]) *xs=cr[i][1];
}

fast
void
strrepf(c xs[st 1],s (*f)(s),s r)
{
	for(;*xs;xs++) if(f(*xs)) *xs=r;
}

fast
c *
strjoin(c *xs[st 1],uz len,fx c join[st 1])
{
	uz i;
	uz size_total=0;
	uz size_mul_temp=0;
	uz len_join=strlen(join);
	uz offset=0;
	s temp;
	c *ret=0;
	for(i=0;i<len;i++){
		if(!xs[i])
continue;
		if(of_addz(strlen(xs[i]),size_total,&size_total))
goto fail;
	}
	if(of_mulz(len-1,len_join,&size_mul_temp))
goto fail;
	if(of_addz(size_mul_temp,size_total,&size_total))
goto fail;
	if(size_total==ZX)
goto fail;
	size_total++;
	ret=malloc(size_total);
	if(!ret)
goto fail;
	for(i=0;i<len-1;i++){
		if(!xs[i])
continue;
		temp=snprintf(ret+offset,size_total-offset,"%s%s",xs[i],join);
		if(temp<0)
goto fail;
		offset+=(uz)temp;
	}
	if(snprintf(ret+offset,size_total-offset,"%s",xs[len-1]?xs[len-1]:"")<0)
goto fail;
	return ret;
fail:
	free(ret);
	return 0;
}

fast
c *
stradvance(fx c xs[st 1])
{
	if(*xs)
xs++;
	if(!*xs)
return 0;
	return(c *)xs;
}

fast
c *
stradvancen(fx c xs[st 1],uz n)
{
	while(n--){
		xs=stradvance(xs);
		if(!xs)
return 0;
	}
	return(c *)xs;
}

fast
c *
stradvanceutf8(fx c xs[st 1])
{
	if(!*xs)
return 0;
	if((*xs++&0xc0)!=0x80)
while(*xs&&(*xs++&0xc0)==0x80);
	if(!*xs)
return 0;
	return(c *)xs;
}

fast
c *
stradvancenutf8(fx c xs[st 1],uz n)
{
	if(!*xs)
return 0;
	while(n--&&*xs) if((*xs++&0xc0)!=0x80) while(*xs&&(*xs++&0xc0)==0x80);
	if(!*xs)
return 0;
	return(c *)xs;
}

fast
void
process_async_spawn(fx c file[st 1],c **argv)
{
	pid_t id;

	id=fork();
	if(id==-1) {
HCF("!: failure to spawn child process %s",file);
	}else if(id==0){
id=fork();
if(id==-1){
	HCF("!: failure to spawn child of child process %s",file);
	exit(EXIT_FAILURE);
}else if(id==0){
	s i;
	setsid();
	setpgid(0,0);
	for(i=sysconf(_POSIX_OPEN_MAX);i>2;--i) close(i);
	execvp(file,argv);
	exit(EXIT_FAILURE);
}
	exit(EXIT_FAILURE);
	}else
wait(&id);
	}

fast
void
process_async_shell_posix(fx c cmd[st 1])
{
	process_async_spawn("sh",(c *[]){"sh","-c",(c *)cmd,0});
}
/* }}} */
fx c xs_aristocratic_bureaucracy[]="\
Copyright (c) © 2021 Muffindrake <muffindrake@protonmail.ch>\n\
\n\
Permission is hereby granted, free of charge, to any person obtaining a copy of\n\
this software and associated documentation files (the \"Software\"), to deal in\n\
the Software without restriction, including without limitation the rights to\n\
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies\n\
of the Software, and to permit persons to whom the Software is furnished to do\n\
so, subject to the following conditions:\n\
\n\
The above copyright notice and this permission notice shall be included in all\n\
copies or substantial portions of the Software.\n\
\n\
THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n\
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n\
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n\
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n\
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n\
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n\
SOFTWARE.\n\
";

fx c xs_help[]="\
usage: wr <argument>\n\
possible arguments: start stop status help version copyright\n\
\tstart: start new attempt, discarding current one\n\
\tstop: discard current attempt\n\
\tstatus: print current attempt elapsed time\n\
\thelp: display this information\n\
\tversion: print version\n\
\tcopyright: print copyright notice\n\
\n\
usage notes:\n\
Timestamps are stored in a file 'worstrun.timer' in the current directory.\n\
The program uses a monotonic clock of the system, so timers do not make sense across restarts of the system.\n\
In such a case, simply delete the timer file in the current directory, or use the stop command.\n\
";

fx c xf_base[]="worstrun.timer";

struct timer{
	s64 sec;
	s64 nsec;
};

fast
struct timer
time_current(void)
{
	struct timespec ts;
	clock_t clock;
#ifdef CLOCK_BOOTTIME
	clock=CLOCK_BOOTTIME;
#else
	clock=CLOCK_MONOTONIC;
#endif
	clock_gettime(clock,&ts);
	return(struct timer){.sec=ts.tv_sec,.nsec=ts.tv_nsec};
}

void
startattempt(void)
{
	FILE *f;
	struct timer t=time_current();
	errno=0;
	f=fopen(xf_base,"w");
	if(!f){
HCF("!: unable to open/create timer file: %s",strerror(errno));
__die();
	}
	fprintf(f,"%"PRId64"\t%"PRId64"\n",t.sec,t.nsec);
	fclose(f);
}

void
discardattempt(void)
{
	FILE *f;
	if(!fileexists(xf_base))
return;
	errno=0;
	f=fopen(xf_base,"w");
	if(!f){
HCF("!: unable to clear timer file: %s",strerror(errno));
__die();
	}
	fclose(f);
}

void
statusattempt(void)
{
	FILE *f;
	struct timer t0=time_current();
	struct timer t1;
	s n;
	if(!fileexists(xf_base))
return;
	errno=0;
	f=fopen(xf_base,"r");
	if(!f){
HCF("!: unable to open/create timer file: %s",strerror(errno));
__die();
	}
	n=fscanf(f,"%"SCNd64"\t%"SCNd64"\n",&t1.sec,&t1.nsec);
	fclose(f);
	if(n!=2)
return;
	t0.sec-=t1.sec;
	t0.nsec-=t1.nsec;
	if(t0.nsec<0){
t0.sec--;
t0.nsec+=1000000000;
	}
	printf("elapsed time %"PRId64":%02"PRId64":%02"PRId64".%03"PRId64"\n",t0.sec/3600,t0.sec%3600/60,t0.sec%60,t0.nsec/1000000);
}

void
version(void)
{
	puts("worstrun version " VERSION);
}

void
help(void)
{
	fputs(xs_help,stdout);
}

void
copyright(void)
{
	version();
	fputs(xs_aristocratic_bureaucracy,stdout);
}

void
unrecognized(void)
{
	puts("command not recognized, try help");
}

s
main(s ac,c **av)
{
#ifndef __COSMOPOLITAN__
	setlocale(LC_ALL,"");
#endif
	if(ac!=2){
HCF("!: require one argument, got %d",ac-1);
return 1;
	}
	if(!strcasecmp(av[1],"status"))
statusattempt();
	else if(!strcasecmp(av[1],"start"))
startattempt();
	else if(!strcasecmp(av[1],"stop"))
discardattempt();
	else if(!strcasecmp(av[1],"version"))
version();
	else if(!strcasecmp(av[1],"help"))
help();
	else if(!strcasecmp(av[1],"copyright"))
copyright();
	else
unrecognized();
	return 0;
}
