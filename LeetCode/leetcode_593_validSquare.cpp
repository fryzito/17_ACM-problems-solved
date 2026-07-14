#include <bits/stdc++.h>
using namespace std;
// Estructura de Vector
struct vec{ 
	int x,y;
	vec(int _x, int _y) {x = _x, y = _y;} 
};

// Hipotenusa al cuadrado en base a los lados más pequeños
long long dist_squared(int p1_x, int p1_y, int p2_x, int p2_y) {
    long long dx = p1_x - p2_x;
    long long dy = p1_y - p2_y;
    return dx*dx + dy*dy;
}

// convertir punto a vector
vec P2Vector (vector<int>& p1, vector<int>& p2) {
	return vec(p2[0] - p1[0] , p2[1] - p1[1]);
}

// Es Angulo Recto
bool AnguIsRecto(vector<int>& A, vector<int>& B, vector<int>& C, vector<int>& D) {
	vec v1 = P2Vector(A,B);
	vec v2 = P2Vector(A,C);
	vec v3 = P2Vector(A,D);

	if((v2.x*v1.x) + (v1.y*v2.y) == 0) return true;
	if((v2.x*v3.x) + (v2.y*v3.y) == 0) return true;
	if((v1.x*v3.x) + (v1.y*v3.y) == 0) return true;

	return false;
}

bool validSquare(vector<int>& p1, vector<int>& p2, vector<int>& p3, vector<int>& p4) {
    set<long long> dist;
    dist.insert(dist_squared(p1[0],p1[1],p2[0],p2[1]));
    dist.insert(dist_squared(p1[0],p1[1],p3[0],p3[1]));
    dist.insert(dist_squared(p1[0],p1[1],p4[0],p4[1]));
    dist.insert(dist_squared(p2[0],p2[1],p3[0],p3[1]));
    dist.insert(dist_squared(p2[0],p2[1],p4[0],p4[1]));
    dist.insert(dist_squared(p3[0],p3[1],p4[0],p4[1]));
    
    if(dist.find(0LL)!=dist.end()) {
        dist.erase(0LL);
    }
    int lados = dist.size();
    int rectos = 0;
	if(AnguIsRecto(p1,p2,p3,p4)) rectos++;
    if(AnguIsRecto(p2,p3,p4,p1)) rectos++;
    if(AnguIsRecto(p3,p4,p1,p2)) rectos++;
    if(AnguIsRecto(p4,p1,p2,p3)) rectos++;
	
    if (lados == 2 && rectos == 4) {
        return true;
    } else {
        return false;
    }
}

int main(){
    int cases, TotalCases;
    int x,y;

    cin >> cases;
    TotalCases = cases;
    while(cases--){
        vector<vector<int>> input;
        // Se leen 4 puntos
        for(int i=0;i<4;i++){
            vector<int> p;
            cin >> x >> y;
            p.push_back(x);
            p.push_back(y);
            input.push_back(p);
        }
        if(validSquare(input[0],input[1],input[2],input[3])){
            printf("Caso %d: Si es cuadrado!\n",TotalCases-cases);
        } else {
            printf("Caso %d: Inminente destruccion!\n",TotalCases-cases);
        }
    }

    return 0;
}