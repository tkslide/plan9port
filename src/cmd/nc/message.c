#include "a.h"

enum{ 
	Border = 1, 
	Padding = 8, 
};

/*
static
int
max(int a, int b)
{
	return a>b ? a : b;
}
*/

void
button(Image *b, Rectangle br, char *label, int focus)
{
	Point p;
	int dx;

	dx = (Dx(br) - stringwidth(font, label)) / 2;
	border(b, br, 2, focus ? cols[Ctitle] : cols[Cdlgbord], ZP);
	p = addpt(br.min, Pt(dx, 0.25*font->height));
	p = stringn(b, p, cols[Ctitle], ZP, font, label, 1);
	string(b, p, cols[Cfg], ZP, font, label+1);
}

int
message(int type, const char *message, Mousectl *mctl, Keyboardctl *kctl)
{
	Alt alts[3];
	Rectangle r, br, brn, sc;
	Point o, p;
	Image *b, *save, *bg, *fg, *hi;
	int rc, done, h, w, bw, bh, mw, focus;
	Mouse m;
	Rune k;

	alts[0].op = CHANRCV;
	alts[0].c  = mctl->c;
	alts[0].v  = &m;
	alts[1].op = CHANRCV;
	alts[1].c  = kctl->c;
	alts[1].v  = &k;
	alts[2].op = CHANEND;
	alts[2].c  = nil;
	alts[2].v  = nil;
	while(nbrecv(kctl->c, nil)==1)
		;
	focus = 0;
	rc = Bno;
	bg = cols[Cdlgbg];
	fg = cols[Cfg];
	hi = type == Derror ? cols[Cerror] : cols[Ctitle];
	done = 0;
	save = nil;
	bw = 3*stringwidth(font, "Yes");
	bh = 1.5*font->height;
	h = Border+Padding+2*font->height+Padding+bh+Padding+Border;
	mw = stringwidth(font, message);
	w = Border+Padding+1.5*mw+Padding+Border;
	b = screen;
	sc = b->clipr;
	replclipr(b, 0, b->r);
	while(!done){
		o = addpt(screen->r.min, Pt((Dx(screen->r)-w)/2, (Dy(screen->r)-h)/2));
		r = Rect(o.x, o.y, o.x+w, o.y+h);
		if(save==nil){
			save = allocimage(display, r, b->chan, 0, DNofill);
			if(save==nil)
				break;
			draw(save, r, b, nil, r.min);
		}
		draw(b, r, bg, nil, ZP);
		border(b, r, Border, cols[Cdlgbord], ZP);
		p = addpt(o, Pt(0, 2));
		line(b, p, Pt(r.max.x, p.y), 0, 0, 2, hi, ZP);
		p = addpt(o, Pt(Border+Padding, Border+Padding+0.5*font->height));
		string(b, p, fg, ZP, font, message);
		p.y += Padding+1.5*font->height;
		br = rectaddpt(Rect(0, 0, bw, bh), addpt(o, Pt(Border+Padding, Border+Padding+2*font->height+Padding)));
		button(b, br, type == Dconfirm ? "Yes" : "Ok", focus == 0);
		if(type == Dconfirm){
			brn = rectaddpt(br, Pt(bw+Padding, 0));
			button(b, brn, "No", focus == 1);
		}
		flushimage(display, 1);
		if(b!=screen || !eqrect(screen->clipr, sc)){
			freeimage(save);
			save = nil;
		}
		b = screen;
		sc = b->clipr;
		replclipr(b, 0, b->r);
		switch(alt(alts)){
		default:
			continue;
			break;
		case 1:
			if(k == '\t')
				focus = (focus + 1) % 2;
			else if((type == Dinfo || type == Derror) && (k == 'o' || k == 'O')){
				done = 1;
				rc = Byes;
			}else if(type == Dconfirm && (k == 'y' || k == 'Y')){
				done = 1;
				rc = Byes;
			}else if(k == '\n'){
				done = 1;
				rc = focus == 0 ? Byes : Bno;
			}else if(type == Dconfirm && (k == 'n' || k == 'N')){
				done = 1;
				rc = Bno;
			}else if(k == Kesc){
				done = 1;
				rc = Bno;
			}
			break;
		case 0:
			if(m.buttons&4){
				if(ptinrect(m.xy, br)){
					done = 1;
					rc = Byes;
				}else if(type == Dconfirm && ptinrect(m.xy, brn)){
					done = 1;
					rc = Bno;
				}
			}
			break;
		}
		if(save){
			draw(b, save->r, save, nil, save->r.min);
			freeimage(save);
			save = nil;
		}
			
	}
	replclipr(b, 0, sc);
	flushimage(display, 1);
	return rc;
}

void
errormessage(char *msg, Mousectl *mc, Keyboardctl *kc)
{
	char errbuf[64+ERRMAX] = {0};
	
	snprint(errbuf, sizeof errbuf, msg);
	message(Derror, errbuf, mc, kc);
}
