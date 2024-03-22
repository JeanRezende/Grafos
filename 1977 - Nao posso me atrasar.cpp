//==================================================================//
/// Grafos para a disciplina de Tecnicas de Programacao            ///
/// IFMG Bambui - Engenharia de Computacao 2023                    ///
/// Alunos:                                                        ///
/// 0034077 - Felipe de Freitas Silva                              ///
/// 0036703 - Jean Gustavo Ferreira Rezende                        ///
//==================================================================//
/*
A entrada desse problema fornece nomes de entrada, eles são registrados cada um
com um indice unico, e para cada vertice foi criado uma lista de adjacencia que
armazena um pair<int,int>, sendo o primeiro numero o destino, e o segundo o peso
da aresta.
Então para descobrir o menor caminho rodamos o dikstra, que recebe um grafo e
o vertice de origem e o inicializa. A partir dai o vertice é colocado em uma fila
de prioridades ordenadas pelo peso, então o vertice de menor custo sempre é explorado
primeiro. Para cada vertice é verificado todos seus adjacentes, e feito o relaxamento,
caso o valor de peso atual seja menor que o peso do vertice de origem + o custo do caminho,
é atualizado sua distancia. Cada vez que um vertice é atualizado é colocado de volta na fila.
Apos o dijkstra temos os menores custos da origem a qualquer vertice.
Então para formatar a saida do problema pegamos o valor passado(X) que é o tempo que
demora do trabalho ao ponto, que vai ser definido para 30 ou 50 min, e somamos o
tempo necessario para chegar ao ponto com nome "alto". A formatação das horas
e se irá se atrasar foi a parte mais cansativa com um monte de restrições e
conversão para string.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

class vertice
{
private:
    string nome = "";
    int id = 0; //identificador
    int distancia = 100000; //soma dos tempos
    int antecessor = -1;
    vector<pair<int, int>> adj; //vetor de dependencias lista de adj
public:
    void setNome(string nome) {this->nome = nome;}
    string getNome() {return this->nome;}
    void setId(int id) {this->id = id;}
    int getId() {return this->id;}
    void setDistancia(int distancia) {this->distancia = distancia;}
    int getDistancia() {return this->distancia;}
    void setAntecessor(int antecessor) {this->antecessor = antecessor;}
    int getAntecessor() {return this->antecessor;}
    void setAdjacente(int id, int peso){adj.push_back(make_pair(id, peso));}
    vector<pair<int, int>> const &getAdjacentes() const {return adj;}
    bool operator<(const vertice& a) const;

};
//função de comparação para utilizar o sort da biblioteca algorithms
 bool vertice::operator<(const vertice& a) const
 { //compara a distancia de 2 instancias
     return distancia < a.distancia;
 }

class grafo
{
private:
    vector<vertice> pontos;
    vector<string> names;
public:

    int setPonto(string nome);
    vector<vertice> &getPontos() {return this->pontos;}
    vertice &getUnicoPonto(int pos) {return this->pontos[pos];}
    vector<string> const &getNames() {return this->names;}
    void clear() {pontos.clear(); names.clear();}
};

//funcão que busca um id unico para cada nome
int grafo::setPonto(string nome)
{
    for( int i = 0; i < getNames().size(); i ++) //percorre a lista de nomes
    {
        if(names[i] == nome) //se o nome for igual retorna sua posição
        {
            return i;
        }
    }
    //caso não encontre, adicione no final da lista de nomes
    names.push_back(nome);

    vertice aux;
    aux.setId(names.size() - 1);
    aux.setNome(nome);
    //adiciona o ponto na lista de vertices
    pontos.push_back(aux);
    //retorna a posição
    return names.size() - 1;
}

void dijkstra(grafo& G, vertice& s) //recebe o grafo G, o vertice s(ource)
{
    priority_queue<vertice> fila; //fila de prioridades ordenadas pelo peso das arestas
    vertice aux; //auxiliar

    s.setDistancia(0); //initialize single source
    fila.push(s); //coloca na fila

    while(!fila.empty())
    {
        aux = fila.top(); //aux recebe o primeiro elemento da fila
        fila.pop(); //remove da fila

        //relaxamento
        for(pair<int,int> v: G.getUnicoPonto(aux.getId()).getAdjacentes()) //percorre a lista de adjacentes desse ponto
        {
            // se a distancia do ponto for maior que a destancia do segundo ponto + a distancia entre a aresta substitui
            if(G.getUnicoPonto(v.first).getDistancia() > (v.second + G.getUnicoPonto(aux.getId()).getDistancia()))
            {
                G.getUnicoPonto(v.first).setDistancia(v.second + G.getUnicoPonto(aux.getId()).getDistancia()); //atualiza para distancia menor
                G.getUnicoPonto(v.first).setAntecessor(aux.getId()); //atualiza o antecessor do atual
                fila.push(G.getUnicoPonto(v.first)); //coloca o vertice atualizado na fila para explorar seus adjacentes
            }
        }
    }
}

//função que formata e retorna a resposta
string resposta(int x, grafo G)
{
    string respostaFormatada = "";
    int tempoTotal = 0;
    if(x <= 30) //se chegou no ponto até 17:30 ou atrasou
    {
        tempoTotal = 30;
    }else{
        tempoTotal = 50;
    }

    for(vertice v: G.getPontos()) //busca o ponto final com o nome alto
    {
        if(v.getNome() == "alto")
        {
            tempoTotal += v.getDistancia(); //soma a distancia dele ao tempo total
        }
    }

    if(tempoTotal >= 60)
    {
        int horas = 17 + tempoTotal/60;
        int minutos = tempoTotal % 60;

        if(minutos == 0 && horas == 18 )
        {
            respostaFormatada += "18:0" + to_string(minutos) + "\n";
            respostaFormatada += "Nao ira se atrasar";
            return respostaFormatada;
        }

        if(minutos < 10)
        {
            respostaFormatada += to_string(horas) + ":0" + to_string(minutos) + "\n";
        }else{
            respostaFormatada += to_string(horas) + ":" + to_string(minutos) + "\n";
        }
        respostaFormatada += "Ira se atrasar";

    }else{
        respostaFormatada += "17:" + to_string(tempoTotal) + "\n";
        respostaFormatada += "Nao ira se atrasar";
    }
    return respostaFormatada;
}



int main()
{
    int x, n, v, t, p, m, a;
    string o, d;
    grafo G;

    cin >> x >> n >> v;

    while(!( x==0 && n==0 && v==0)){

        for(int i = 0; i < n; i++)
        {
            cin >> o >> d >> t;

            //todos os nomes são colocados na lista
            p = G.setPonto(o);
            m = G.setPonto(d);
            //lista de adjacente cresce da origem
            G.getPontos()[p].setAdjacente(m,t);
        }

        //descobre a posição que do ponto inicial do grafo
        for(int i= 0; i < G.getNames().size(); i++)
        {
            if(G.getNames()[i] == "varzea")
            {
                a = i;
            }
        }
        //chama o dijkstra passando o grafo, o vertice de inicio
        dijkstra(G, G.getUnicoPonto(a));
        //apos a descobrir o menor caminho a resposta é formatada e mostrada
        cout << resposta(x, G) << endl;

        cin >> x >> n >> v;

        //limpeza dos vetores
        G.clear();
    }
    return 0;
}
