#include "a.h"

Rectangle
boundsrect(Rectangle r)
{
	return Rect(0, 0, Dx(r), Dy(r));
}

Image*
ealloccolor(ulong col)
{
	Image *b;
	
	b = allocimage(display, Rect(0, 0, 1, 1), screen->chan, 1, col);
	if(b == nil)
		sysfatal("allocimage: %r");
	return b;
}

void*
emalloc(ulong n)
{
	void *p;
	
	p = malloc(n);
	if(p == nil)
		sysfatal("malloc: %r");
	return p;
}

void*
erealloc(void *p, ulong n)
{
	void *t;
	
	t = realloc(p, n);
	if(t == nil)
		sysfatal("realloc: %r");
	return t;
}

char*
slurp(char *path)
{
	int fd;
	long r, n, s;
	char *buf;

	n = 0;
	s = 8192;
	buf = malloc(s);
	if(buf == nil)
		return nil;
	fd = open(path, OREAD);
	if(fd < 0)
		return nil;
	for(;;){
		r = read(fd, buf + n, s - n);
		if(r < 0)
			return nil;
		if(r == 0)
			break;
		n += r;
		if(n == s){
			s *= 1.5;
			buf = realloc(buf, s);
			if(buf == nil)
				return nil;
		}
	}
	buf[n] = 0;
	close(fd);
	return buf;
}

char *
homedir(char *user)
{
	static char buf[1024];
	struct passwd* pw;

	setpwent();
	while((pw = getpwent()) != nil)
		if(strcmp(user, pw->pw_name) == 0){
			strecpy(buf, buf+sizeof buf, pw->pw_dir);
			return buf;
		}
	return nil;
}

char*
abspath(char *wd, char *p)
{
	char *s;

	if(p[0]=='/')
		s = strdup(p);
	else
		s = smprint("%s/%s", wd, p);
	cleanname(s);
	return s;
}

int
wresize(int w, int h)
{
	int fd, n;
	char buf[255];

	fd = open("/dev/wctl", OWRITE|OCEXEC);
	if(fd < 0)
		return -1;
	n = snprint(buf, sizeof buf, "resize -dx %d -dy %d", w, h);
	if(write(fd, buf, n) != n){
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}
