#define M_PI 3.1415926535897932384626433832795
#pragma warning(disable : n)

// unidades de medida
const double milenio = 1;
const double AL = 1.0; const float UA = 0.0000158125 * AL;  const float metro = UA / 1.496e11; const float newton = 1.0; const double mSol = 1.0;
const float kg = mSol / 1.89e30;
//const float G = 0.00005;
//const float G = 6.67e-11 * newton * metro * metro / kg;
const float G = 1.34e-17 * AL * AL * AL / mSol / (milenio * milenio);
const float r_soft = 1e9*AL;

const int n = 25000;
const float massa = 1e12/n*mSol;
float cores_corpos[n][3];
float espacamento = 110e3*AL;

const float dt = 8e6*milenio;

struct vec3 {
    float x, y, z;
    vec3() : x(0.0), y(0.0), z(0.0){}
    vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_){}
};

float rng() {
    return rand() / double(RAND_MAX);
}

void redimensiona(int largura, int altura) {
    glViewport(0, 0, largura, altura);
    glMatrixMode(GL_PROJECTION);
    float aspect = (float)largura / (float)altura;

    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 500.0);

    glMatrixMode(GL_MODELVIEW);
}

void rect(vec3 p1, vec3 p2, vec3 p3, vec3 p4, color cor) {
    glColor3fv(cor);
    glBegin(GL_QUADS);
    glVertex3fv(&p1.x); // pode trocar por vetor[3] = { p1.x, p1.y, p1.z };
    glVertex3fv(&p2.x); // de alguma forma, apesar de eu botar apenas as chaves da coordenada x, ele referencia todas as coordenadas
    glVertex3fv(&p3.x);
    glVertex3fv(&p4.x);
    glEnd();
}

vec3 polarToCartesian(float r, float theta, float phi) {
    vec3 vetor;
    vetor.x = r * cos(theta) * sin(phi);
    vetor.y = r * sin(theta) * sin(phi);
    vetor.z = r * cos(phi);
    return vetor;
}

void desenhaEsfera(double nod, float r) {
    static float step = M_PI / nod;
    for (float theta = 0; theta < 2 * M_PI - step; theta = theta + step) {
        for (float phi = 0; phi < M_PI - step; phi = phi + step) { // talvez tenha que subtrair por M_PI / nod
            vec3 v1 = polarToCartesian(r, theta, phi);
            vec3 v2 = polarToCartesian(r, theta + step, phi);
            vec3 v3 = polarToCartesian(r, theta + step, phi + step);
            vec3 v4 = polarToCartesian(r, theta, phi + step);
            color cor = { theta / M_PI,phi / M_PI,1 - theta / M_PI };
            rect(v1, v2, v3, v4, cor);
        }
    }


    float theta = 0.0;
    float phi = 0.0;
}

void desenhaCubo(float tamanho) {
    float d = tamanho / 2.0;
    vec3 v1(-d, d, d);
    vec3 v2(-d, -d, d);
    vec3 v3(d, -d, d);
    vec3 v4(d, d, d);
    vec3 v5(d, d, -d);
    vec3 v6(d, -d, -d);
    vec3 v7(-d, -d, -d);
    vec3 v8(-d, d, -d);

    rect(v1, v2, v3, v4, orchid);
    rect(v4, v3, v6, v5, azul);
    rect(v5, v8, v7, v6, darkorchid);
    rect(v1, v8, v7, v2, blueviolet);
    rect(v1, v4, v5, v8, slateblue);
    rect(v2, v7, v6, v3, lilas);


}

double rngforpdf(double (*pdf)(double), float rinic, float rfin, int iteracoes){
  for (int i = 0; i < iteracoes; i++){
    double rand_x = (rfin - rinic) * rng() + rinic;
    double rand_y = rng();
    double calc_y = pdf(rand_x);
  if (rand_y<=calc_y){
    return rand_x;}
  }
  return -1;
  }

double dens_r(double r){
  return exp(-r/espacamento);
}


void desenhaPonto(float r,vec3 p,colora cor) {
    glColor4fv(cor);
    glPointSize(r);
    
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
        glVertex3fv(&p.x);
    glEnd();
}

float Fgravitacional(float m2, vec3 p1, vec3 p2) {
    float r2 = pow(p1.x - p2.x,2) + pow(p1.y - p2.y,2) + pow(p1.z - p2.z,2);
    float F = -G * m2 / (r2 + r_soft);
    return F;
}
vec3 velocidade(float F, vec3 v1, vec3 p1, vec3 p2) {
    float dx, dy, dz; dx = p1.x - p2.x; dy = p1.y - p2.y; dz = p1.z - p2.z;
    //vec3 d(dx, dy, dz);
    float r = sqrt(dx * dx + dy * dy + dz * dz);
    vec3 a(F * dx / r, F * dy / r, F * dz / r);
    vec3 v(v1.x + a.x*dt, v1.y + a.y*dt, v1.z + a.z*dt);
    return v;

}
vec3 aceleracao(float F, vec3 p1, vec3 p2) {
    float dx, dy, dz; dx = p1.x - p2.x; dy = p1.y - p2.y; dz = p1.z - p2.z;
    //vec3 d(dx, dy, dz);
    float r = sqrt(dx * dx + dy * dy + dz * dz);
    vec3 a(F * dx / r, F * dy / r, F * dz / r);
    return a;

}

struct corpo {
    float massa;
    float pos[3];
    float vel[3];
    float acc[3];
    bool exist;
};


// orden: massa, x, y, z, vx,vy, vz, ax, ay, az, exist
corpo corpos[n];
void inicializarCorpos() {
    float magnitudev = 1;
    for (int i = 0; i < n; i++) {
        float raiocorpo = rngforpdf(dens_r, 0, espacamento, 10000);
        float angulocorpo = rng() * 2 * M_PI;
        //float posx = 2 * (rng() - 0.5) * espacamento; float posy = 2 * (rng() - 0.5) * espacamento; float raiocorpo = sqrt(posx * posx + posy * posy); float angulocorpo = atan2(posy, posx);
        // Atribuição correta, elemento por elemento
        corpos[i].massa = massa;
        corpos[i].pos[0] = raiocorpo * cos(angulocorpo); // rng() * espacamento_x - espacamento_x/2;
        corpos[i].pos[1] = raiocorpo * sin(angulocorpo);// rng()* espacamento_y - espacamento_y / 2;
		corpos[i].pos[2] = -200 + rng() * 0.01;
		corpos[i].vel[0] = -sin(angulocorpo) * sqrt(G * massa * n * raiocorpo / pow(espacamento, 2)) * magnitudev;
		corpos[i].vel[1] = cos(angulocorpo) * sqrt(G * massa * n * raiocorpo / pow(espacamento, 2)) * magnitudev;
		corpos[i].vel[2] = 0;
		corpos[i].acc[0] = 0; corpos[i].acc[1] = 0; corpos[i].acc[2] = 0;
		corpos[i].exist = true;
        
        cores_corpos[i][0] = 0.8 + 0.2*cos(2*angulocorpo); cores_corpos[i][1] = 0.6;  cores_corpos[i][2] = 0.8 + 0.2*sin(2*angulocorpo);
        //cores_corpos[i][0] = 0; cores_corpos[i][1] = 0;  cores_corpos[i][2] = 0;
    }
    
}



// inicializarCorpos() é executado na main, porque a primitivas.h não pode realizar nenhum laço ou algo do tipo


// massa, (x,y,z),(vx,vy,vz)
//vec3 p1(1, 1, -10); vec3 v1(-0.001, 0.001, 0);
//vec3 p2(-1, -1, -10); vec3 v2(0.002, 0, 0);

bool leap = 0;

void desenhag() {
	using namespace std;
    float m1, m2;
    m1 = 1; m2 = 0.7;
    float momento[3] = { 0,0,0 };
    
    #pragma omp parallel for
    for (int i = 0;i < n;i++) { // i é o que sofre a força
        vec3 p1(corpos[i].pos[0], corpos[i].pos[1], corpos[i].pos[2]); vec3 v1(corpos[i].vel[0], corpos[i].vel[1], corpos[i].vel[2]);
        vec3 a1(0, 0, 0);
		corpos[i].acc[0] = 0; corpos[i].acc[1] = 0; corpos[i].acc[2] = 0;
        //#pragma omp parallel for
        for (int j = 0; j < n; j++) {
            if (i != j && corpos[i].exist == true && corpos[j].exist == true) {
				vec3 p2(corpos[j].pos[0], corpos[j].pos[1], corpos[j].pos[2]);
				vec3 v2(corpos[j].vel[0], corpos[j].vel[1], corpos[j].vel[2]);
                float a = Fgravitacional(corpos[j].massa, p1, p2);
                a1 = aceleracao(a, p1, p2); // cálculos mostraram que implementar direto na velocidade dá no mesmo q na aceleração
				corpos[i].acc[0] = corpos[i].acc[0] + a1.x; corpos[i].acc[1] = corpos[i].acc[1] + a1.y; corpos[i].acc[2] = corpos[i].acc[2] + a1.z;
				
                //v1 = velocidade(F, v1, p1, p2);
                //v2 = velocidade(-F, m2, v2, p2, p1);
                // atualizar posições
                //p1.x = p1.x + v1.x*dt; p1.y = p1.y + v1.y*dt; p1.z = p1.z + v1.z*dt;
                //corpos[i][1] = p1.x; corpos[i][2] = p1.y; corpos[i][3] = p1.z; <----- agora é feito um loop só para atualizar a posição dos corpos
                //corpos[i][4] = v1.x; corpos[i][5] = v1.y; corpos[i][6] = v1.z;
                //corpos[j][1] = p2.x; corpos[j][2] = p2.y; corpos[j][3] = p2.z;
                
                
        }}
        glLoadIdentity();
        
        //for (int k = 0; k < 3; k++) {
        //    momento[k] = momento[k] + corpos[i][0] * corpos[i][k + 4];
        //}
    }
    //#pragma omp parallel for
    for (int i = 0; i < n; i++) {
        if (corpos[i].exist == true) {
            desenhaPonto(2, vec3(corpos[i].pos[0] * 80 / espacamento, corpos[i].pos[1] * 80 / espacamento, corpos[i].pos[2]), cores_corpos[i]);
		}
        vec3 p1(corpos[i].pos[0], corpos[i].pos[1], corpos[i].pos[2]); vec3 v1(corpos[i].vel[0], corpos[i].vel[1], corpos[i].vel[2]);
		v1.x = v1.x + corpos[i].acc[0] * dt; v1.y = v1.y + corpos[i].acc[1] * dt; v1.z = v1.z + corpos[i].acc[2] * dt;
        p1.x = p1.x + v1.x*dt; p1.y = p1.y + v1.y*dt; p1.z = p1.z + v1.z*dt;
        corpos[i].vel[0] = v1.x; corpos[i].vel[1] = v1.y; corpos[i].vel[2] = v1.z;
        corpos[i].pos[0] = p1.x; corpos[i].pos[1] = p1.y; corpos[i].pos[2] = p1.z;

    }

    //std::cout << corpos[0][1] << " " << corpos[1][1] << std::endl;

    //std::cout << momento[0] << " " << momento[1] << " " << momento[2] << std::endl;
    //desenhaPonto(10, vec3(corpos[i][1], corpos[i][2], corpos[i][3]), azul);
    //desenhaPonto(10, vec3(corpos[j][1], corpos[j][2], corpos[j][3]), verde);
    //std::cout << p1.x << " " << p2.x << std::endl;
    //for (int cont = 0; cont < 200000000; cont++){}
}

/// Fim do teste

void desenha(float num) {
    glLoadIdentity();
    desenhaPonto(10, vec3(num * 0.001, 0, -10.0),azul);
    std::cout << num * 0.001 << std::endl;
}

void desenhaOld(float num) {
    glLoadIdentity();

    glPushMatrix();
        glTranslated(0.0, 0.0, -10.0);
        glRotated(30.0 + num, 15.0 + 4 * sin(num / 40), 5.0, 10.0);
        //desenhaEsfera(12, 3);
    glPopMatrix();
    

    glPushMatrix();
        glTranslated(0.0, 0.0, -50.0);
        glRotated(30.0 + num, 15.0 + 4 * sin(num / 40), 5.0, 10.0);
        glScalef(1 + 0.1 * sin(num / 5/M_PI), 1 + 0.1 * sin(num / 5/M_PI), 1 + 0.1 * sin(num / 5/M_PI));
        ///desenhaCubo(10);
    glPopMatrix(); // essa função retorna ao eixo de coordenadas anterior (que é o da identidade, nesse caso)

    desenhaPonto(20, vec3(0 , 0, -10 + 4 * sin(num / 10 / M_PI)), amarelo);
}

//volta completa: 1.6e10 milenios
