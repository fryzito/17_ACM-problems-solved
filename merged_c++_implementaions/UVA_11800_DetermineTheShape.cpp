#include<iostream>
#include<cstdio>
#include<set>
#include<vector>
#include<string>
#include<utility>
#include<map>
#include<cmath>
#include<queue>
#include<stack>
#include<cstring>
#include<algorithm>
#include<sstream>
#define ll long long
#define vint vector<int>
#define f(i,x,y) for(int i=x;i<y;i++)
#define fd(i,y,x) for(int i=y;i>=x;i--)
#define clr(A,x) memset(A,x,sizeof(A))
#define oo (1<<30)
#define eps (1e-9)
#define all(v) (v).begin(),(v).end()
#define rall(v) (v).rbegin(),(v).rend()
#define pb push_back
#define sqr(x) (x)*(x)
#define dbg(x) cout << #x << " = " << x << endl;
#define dbg2(x,y) cout << #x << " = " << x << " " << #y << " = " << y << endl;
#define sz size()
using namespace std;
struct point {
	int x, y;
};
struct vec{ 
	double x,y;
	vec(double _x, double _y) {x = _x, y = _y;} 
};
// Points to Vector
vec Tovector (point p1, point p2) {
	return vec(p2.x - p1.x , p2.y - p1.y);
}

// Hipotenusa en base a los lados más pequeños
double hipotenusa(int dx, int dy) { return sqrt(dx*dx + dy*dy);}

// Distancia de dos puntos
double distancia(point p1,point p2) {
	return hipotenusa(p1.x - p2.x,p1.y - p2.y);
}

struct line {	double a, b, c; };

void pointToLine(point p1, point p2, line *l) {
	if(p1.x == p2.x) {
		l->a = 1.0; l ->b = 0.0 ; l->c = -p1.x;
	} else {
		l->a =  -(double)(p1.y - p2.y)/(p1.x-p2.x);
		l->b = 1.0;
		l->c = -(double) (l->a * p1.x) - (l->b*p1.y);
	}
}

bool areParallel(line l1, line l2) {
	return (fabs(l1.a-l2.a) < eps) && (fabs(l1.b-l2.b) < eps);
}

bool aresame(point p1, point p2) {
	return fabs(p1.x - p2.x) < eps && fabs(p1.y - p2.y) < eps;
}

bool Arecto(point A,point B, point C,point D) {
	vec v1 = Tovector(A,B);
	vec v2 = Tovector(A,C);
	vec v3 = Tovector(A,D);
	if((v2.x*v1.x) + (v1.y*v2.y) == 0) return true;
	if((v2.x*v3.x) + (v2.y*v3.y) == 0) return true;
	if((v1.x*v3.x) + (v1.y*v3.y) == 0) return true;
	return false;
}

bool cuatrolados(point p1, point p2, point p3 , point p4) {
	line l1,l2; double lado1 = 0,lado2 = 0, lado3 = 0;
	pointToLine(p1,p2,&l1);
	pointToLine(p3,p4,&l2);
	if(distancia(p1,p2) == distancia(p3,p4) && areParallel(l1,l2) )
		lado1 = distancia(p1,p2);
	pointToLine(p1,p3,&l1);
	pointToLine(p2,p4,&l2);
	if(distancia(p1,p3) == distancia(p2,p4) && areParallel(l1,l2) )
		lado2 = distancia(p1,p3);
	pointToLine(p1,p4,&l1);
	pointToLine(p2,p3,&l2);
	if(distancia(p1,p4) == distancia(p2,p3) && areParallel(l1,l2) )
		lado3 = distancia(p1,p4);
	if(lado1 == 0) {
		if(lado2 == lado3) return true;
		else return false;
	}else if (lado2 == 0) {
		if(lado1 == lado3) return true;
		else return false;
	} else {
		if(lado1 == lado2) return true;
		else false;
	}
}
int main() {
	int nc;
	scanf("%d",&nc);
	point p;
	f(k,0,nc) {
		vector<point> v;
		f(q,0,4) {
			scanf("%d %d",&p.x,&p.y);
			v.pb(p);
		}
		set<double> dist;
		double dis1 = distancia(v[0],v[1]); if(dist.find(dis1)==dist.end()) dist.insert(dis1);
		double dis2 = distancia(v[0],v[2]); if(dist.find(dis2)==dist.end()) dist.insert(dis2);
		double dis3 = distancia(v[0],v[3]); if(dist.find(dis3)==dist.end()) dist.insert(dis3);
		double dis4 = distancia(v[1],v[2]); if(dist.find(dis4)==dist.end()) dist.insert(dis4);
		double dis5 = distancia(v[1],v[3]); if(dist.find(dis5)==dist.end()) dist.insert(dis5);
		double dis6 = distancia(v[2],v[3]); if(dist.find(dis6)==dist.end()) dist.insert(dis6);	
		int lados = dist.sz;
		//
		vector<line> vline;
		line l1, l2, l3, l4, l5, l6;
		pointToLine(v[0],v[1],&l1); vline.pb(l1);
		pointToLine(v[0],v[2],&l2); vline.pb(l2);
		pointToLine(v[0],v[3],&l3); vline.pb(l3);
		pointToLine(v[1],v[2],&l4); vline.pb(l4);
		pointToLine(v[1],v[3],&l5); vline.pb(l5);
		pointToLine(v[2],v[3],&l6); vline.pb(l6);
		int rectos = 0;
		f(i,0,4) {
			if(Arecto(v[i%4],v[(i+1)%4],v[(i+2)%4],v[(i+3)%4]))
				rectos++;
		}
		int paralelos = 0;
		f(i,0,vline.sz)
			f(j,i+1,vline.sz) {
				if(areParallel(vline[i],vline[j]))
					paralelos++;
			}
		bool exit = false;
		f(i,0,v.sz-1) {
			f(j,i+1,v.sz) {				
				if(aresame(v[i],v[j])) {
					exit = true;
					break;
					break;
				}
			}
		}
		if(lados == 2 && rectos == 4 && paralelos == 2) printf("Case %d: Square\n",k+1);
		else if(lados == 3 && rectos == 0 && paralelos == 2 && cuatrolados(v[0],v[1],v[2],v[3])) printf("Case %d: Rhombus\n",k+1);
		else if(lados == 3 && rectos == 4 && paralelos == 2) printf("Case %d: Rectangle\n",k+1);
		else if( paralelos == 2 && rectos == 0 ) printf("Case %d: Parallelogram\n",k+1);
		else if(paralelos == 1 || exit) printf("Case %d: Trapezium\n",k+1);
		else { printf("Case %d: Ordinary Quadrilateral\n",k+1); }
	}
	
	return 0;
}
