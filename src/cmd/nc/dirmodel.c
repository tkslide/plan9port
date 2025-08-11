#include "a.h"

static int
dircmpname(Dir *a, Dir *b)
{
	int cmp;

	if(a->qid.type&QTDIR){
		if(b->qid.type&QTDIR)
			cmp = strcmp(a->name, b->name);
		else
			cmp = -1;
	}else if(b->qid.type&QTDIR){
		cmp = 1;
	}else{
		cmp = strcmp(a->name, b->name);
	}
	return cmp;
}

static void
loadpath(Dirmodel *m)
{
	char buf[1024];
	int fd;
	Dir *t, *d;

	m->isroot = strcmp(m->path, "/") == 0;
	fd = open(m->path, OREAD);
	/* FIXME: error handling */
	m->ndirs = dirreadall(fd, &m->dirs);
	if(m->ndirs > 0)
		qsort(m->dirs, m->ndirs, sizeof *m->dirs,   (int(*)(const void*,const void*))dircmpname);
	close(fd);
	if(!m->isroot){
		t = emalloc((m->ndirs + 1) * sizeof(Dir));
		memmove(&t[1], m->dirs, m->ndirs*sizeof(Dir));
		m->dirs = t;
		m->ndirs++;
		snprint(buf, sizeof buf, "%s/..", m->path);
		d = dirstat(buf);
		memmove(&m->dirs[0], &d[0], sizeof(Dir));
		m->dirs[0].name = "..";
	}
	m->sel = emalloc(m->ndirs * sizeof(uchar));
	memset(m->sel, 0, m->ndirs * sizeof(uchar));
}

void
dirmodelreload(Dirmodel *m)
{
	free(m->dirs);
	free(m->sel);
	loadpath(m);
	sendul(m->c, 1);
}

void
dirmodelreloadifsame(Dirmodel *m, Dirmodel *o)
{
	if(strcmp(m->path, o->path) == 0)
		dirmodelreload(o);
}

void
dirmodelcd(Dirmodel *m, char *p)
{
	char newpath[1024] = {0};

	if(p[0] == '/')
		snprint(newpath, sizeof newpath, "%s", p);
	else
		snprint(newpath, sizeof newpath, "%s/%s", m->path, p);
	if(access(newpath, 0)<0)
		sysfatal("directory does not exist: %r");
	free(m->path);
	m->path = abspath(m->path, newpath);
	free(m->filter);
	m->filter = nil;
	free(m->fdirs);
	m->fndirs = 0;
	dirmodelreload(m);
}

Dirmodel*
mkdirmodel(char *path)
{
	Dirmodel *dm;

	dm = emalloc(sizeof *dm);
	dm->c = chancreate(sizeof(ulong), 1);
	dm->path = strdup(path);
	dm->filter = nil;
	dm->fdirs = nil;
	loadpath(dm);
	return dm;
}

Dir
dirmodelgetdir(Dirmodel *m, int i)
{
	if(m->filter != nil)
		return m->fdirs[i];
	return m->dirs[i];
}

long
dirmodelcount(Dirmodel *m)
{
	if(m->filter != nil)
		return m->fndirs;
	return m->ndirs;
}

void
dirmodelfilter(Dirmodel *m, char *p)
{
	char buf[1024];
	int fd, i;
	Dir *d, *u;
	long n;

	if(p == nil){
		free(m->filter);
		m->filter = nil;
		free(m->fdirs);
		m->fndirs = 0;
		sendul(m->c, 1);
		return;
	}
	fd = open(m->path, OREAD);
	/* FIXME: error handling */
	n = dirreadall(fd, &d);
	if(n > 0)
		qsort(d, n, sizeof *d,  (int(*)(const void*,const void*))dircmpname);
	close(fd);
	m->fdirs = emalloc((n+1) * sizeof(Dir));
	if(!m->isroot){
		snprint(buf, sizeof buf, "%s/..", m->path);
		u = dirstat(buf);
		memmove(&m->fdirs[0], &u[0], sizeof(Dir));
		m->fdirs[0].name = "..";
		m->fndirs++;
	}
	for(i = 0; i < n; i++){
		if((d[i].qid.type&QTDIR) || match(d[i].name, p))
			memmove(&m->fdirs[m->fndirs++], &d[i], sizeof(Dir));
	}
	memset(m->sel, 0, m->ndirs * sizeof(uchar));
	m->filter = strdup(p);
	sendul(m->c, 1);
}

long
dirmodelmarklist(Dirmodel *m, Dir **d)
{
	int i, j;
	long n, ndirs;
	Dir *dirs;

	n = 0;
	ndirs = m->ndirs;
	dirs = m->dirs;
	if(m->filter){
		ndirs = m->fndirs;
		dirs = m->fdirs;
	}
	for(i = 0; i < ndirs; i++)
		n += m->sel[i];
	if(n == 0)
		return 0;
	*d = emalloc(n*sizeof(Dir));
	j = 0;
	for(i = 0; i < ndirs; i++){
		if(m->sel[i])
			(*d)[j++] = dirs[i];
	}
	return n;
}

int
dirmodeleq(Dirmodel *a, Dirmodel *b)
{
	return strcmp(a->path, b->path) == 0;
}
