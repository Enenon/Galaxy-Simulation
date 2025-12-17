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

std::vector<double> suavizar(const std::vector<double>& data, int windowSize) {
    if (data.empty() || windowSize <= 0) return {};

    std::vector<double> smoothed;
    int n = data.size();

    for (int i = 0; i <= n - windowSize; ++i) {
        double sum = 0.0;
        for (int j = 0; j < windowSize; ++j) {
            sum += data[i + j];
        }
        smoothed.push_back(sum / windowSize);
    }
    return smoothed;
}

std::vector<double> exponentialSmoothing(std::vector<double>& data, double alpha) {
    std::vector<double> smoothed = { data[0]};
    if (data.size() < 2) return smoothed;

    // Começamos do segundo elemento (índice 1)
    // Preservando o data[0] como o valor inicial de 'previous'
    double previousSmoothed = data[0];

    for (size_t i = 1; i < data.size(); ++i) {
        double currentSmoothed = alpha * data[i] + (1.0 - alpha) * previousSmoothed;
        smoothed.push_back(currentSmoothed);
        previousSmoothed = currentSmoothed;
    }
    return smoothed;
}