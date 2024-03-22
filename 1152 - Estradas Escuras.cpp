//==================================================================//
/// Grafos para a disciplina de Tecnicas de Programacao            ///
/// IFMG Bambui - Engenharia de Computacao 2023                    ///
/// Alunos:                                                        ///
/// 0034077 - Felipe de Freitas Silva                              ///
/// 0036703 - Jean Gustavo Ferreira Rezende                        ///
//==================================================================//
/*
Nesse codigo separamos em 2 partes, o grafo que armazena as arestas e pesos das estradas
e o conjunto disjunto que são as conexões dos vertices.
Então o kruskal pega a arestas e as ordena de forma crescente de peso, com a função sort,
cada estrada percorrida tentamos fazer a uniao dos vertices no conjunto disjunto, se
os vertices pertencerem a conjuntos diferentes é feita uma união. Portanto é garantido que
tenha um unico caminho para cada vertice e que é o menor possivel devido a ordenação.
O diferencial é que essa Arvore gerada não precisa de nenhuma origem.
O codigo não necessitou nenhuma mudança para se adequar o problema, e a resposta foi formatada
somando o peso de cada união feita, e subtraindo do peso total obtido pela soma da entrada.
mostrando o valor maximo possivel de economia.
O grande problema que tivemos com esse algoritmo foi o while da entrada que deveria ter o 0 0
como parada ao final sem ele o beecrowd so retornava wrong answer
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class aresta
{
private:
    int x,y,peso;
public:
    aresta(int x, int y, int peso){this->x = x; this->y = y; this->peso = peso;}
    void setX(int x) {this->x = x;}
    int getX() {return x;}
    void setY(int y) {this->y = y;}
    int getY() {return y;}
    void setPeso(int peso) {this->peso = peso;}
    int getPeso() {return peso;}
    bool operator<(const aresta &a) const ;
};
//operador de comparação para utilizar o metodo sort da biblioteca algorithm
bool aresta::operator<(const aresta &a) const{
        return peso < a.peso; //sobrescreve e compara o peso
}

class grafo
{
private:
    vector<aresta> estradas {}; //vetor de estradas
public:
    void addEstrada(int x, int y, int z);
    void ordenaEstrada();
    vector<aresta> const &getEstradas() const { return estradas;}
    void clear() {estradas.clear();}
};

void grafo::addEstrada(int x, int y, int z)
{
    estradas.push_back(aresta(x,y,z));
}

void grafo::ordenaEstrada() //função que ordena de forma crescente de peso as arestas do grafo
{
    sort(estradas.begin(), estradas.end()); //organiza na ordem crescente o peso das estradas
}

class conjuntoDisjunto{
private:
    vector<int> conjunto {};
public:
    void makeSet(int m);
    int findSet(int u);
    bool uniao(int u, int v);
    void clear() {conjunto.clear();}
    //vector<int> const &getConjunto() const {return conjunto;}
};

void conjuntoDisjunto::makeSet(int m) //faz o conjunto de 1 ate o numero total, mas começa na pos 0
{
    conjunto.reserve(200000);
    for(int i = 0; i < m; i++)
    {
        conjunto.push_back(i); //conjunto na pos 0 recebe numero 1
    }
}

int conjuntoDisjunto::findSet(int u) //recebe um numero -1 pra corrigir para posicao
{
    if(conjunto[u] != u) //se nao fizer parte do proprio conjunto pode estar em um conjunto de conjunto
    {
        conjunto[u] = findSet(conjunto[u]); //find set recursivo até achar a base
    }
    return conjunto[u]; //retorna o numero do conjunto que ele faz parte
}

bool conjuntoDisjunto::uniao(int u, int v) //so chama se os conjuntos forem diferentes
{
    //inicialização para evitar chamada de funções repetidamente
    int setU = findSet(u);
    int setV = findSet(v);

    if( setU != setV) //compara se estão em conjuntos diferentes
    {
        if(setU < setV) //se o conjunto U for menor a união ocorre em V
        {
            conjunto[setU] = setV; //a união apenas copia uma referencia para a posição
        }else{ //se não a união é em U
            conjunto[setV] = setU;
        }
        return true;
    }
    return false;
}
//kruskal recebe 2 instancias por referencia
//responsavel por calcular a menor distancia que conecta todos os vertices
int kruskal(conjuntoDisjunto& conjD, grafo& G)
{
    int pesoFinal = 0;
    G.ordenaEstrada(); //utiliza um sort para ordenar as arestas por peso crescente
    for(aresta a: G.getEstradas()){ //percorre as arestas
        if(conjD.uniao(a.getX(), a.getY())) //une as arestas que pertencem a conjuntos diferentes
        {
            pesoFinal += a.getPeso(); //se uniu adiciona ao peso de saída
        }
    }
    return pesoFinal;
}

int main()
{
    unsigned int m = 0, n = 0, x = 0, y = 0, z = 0;
    grafo G;
    conjuntoDisjunto conjD;
    int pesoTotal = 0, pesoFinal = 0;
    cin >> m >> n;
    while(!(m == 0 && n == 0)){ //numero de juncoes, numero de estradas

        conjD.makeSet(m); //inicializa o conjunto disjunto começando em 0
        for(unsigned int i = 0; i < n; i++)
        {
            cin >> x >> y >> z;
            G.addEstrada(x, y, z);
            pesoTotal+= z;
        }

        pesoFinal = kruskal(conjD, G); //peso final recebe peso total do menor caminho do kruskal

        cin >> m >> n;
        cout << pesoTotal - pesoFinal << endl; //resposta final

        //limpeza de variaveis
        pesoFinal = 0;
        pesoTotal = 0;
        G.clear();
        conjD.clear();
    }
    return 0;
}
