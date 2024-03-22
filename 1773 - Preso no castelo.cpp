//==================================================================//
/// Grafos para a disciplina de Tecnicas de Programacao            ///
/// IFMG Bambui - Engenharia de Computacao 2023                    ///
/// Alunos:                                                        ///
/// 0034077 - Felipe de Freitas Silva                              ///
/// 0036703 - Jean Gustavo Ferreira Rezende                        ///
//==================================================================//
/*
Utilizamos novamente uma lista de adjacentes, para onde cada sala do castelo
tinha corredor, foi necessario tambem um vetor de chaves pois em cada sala
pode ter mais de uma chave(não explicito no problema).
Esse foi o algoritmo que mais precisou de adaptações para se adequar ao problema,
construimos o Busca em Largura de acordo com o Cormen inicializando de um ponto
e verificando se era possivel acessar e abrir todos os quartos.
O BFS conta com uma fila de vertices a explorar, para cada vertice é explorado
todos seus adjacentes, assim que explorar são marcados por cores para reconhecer
o estado do quarto como não visitado, visitado, ou concluido.
Foi adicionado um teste para saber se o quarto adj estava aberto. Caso estivesse
ele pode abrir as outras portas que ele tem chaves, caso o quarto estiver fechado
adicionamos um bool chamado flag, no final da fila do BFS esses quartos fechados
podem ter sido abertos por outras salas, então se tem essa flag e estão atualmente
abertos é chamado novamente o BFS, mas com esse vertice como raiz, para percorrer
ele e seus adjacentes que poderiam ter ficado esquecidos.
Apos essa execução na função verifica percorre todos os vertices, e caso todos
estejam marcados como completamente explorados(preto) quer dizer que foi possivel
abrir todas as portas, caso não estiver é retornado um "não".
*/


#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class vertice
{
private:
    int numero = 0;
    int cor = 0; // 0 branco, 1 cinza, 2 preto
    bool aberta = false;
    bool flag = false;
    vector<int> adj = {}; //adj.clear()
    vector<int> chave = {};
public:
    vertice(int id){this->numero = id;}
    void setNumero(int numero){this->numero = numero;}
    int getNumero() {return this->numero;}
    void setCor(int cor) {this->cor = cor;}
    int getCor() {return this->cor;}
    void setAberta(bool aberta) {this->aberta = aberta;}
    bool getAberta() {return this->aberta;}
    void setFlag(bool flag) {this->flag = flag;}
    bool getFlag() {return this->flag;}
    void addAdj(int x) {this->adj.push_back(x);}
    vector<int> getAdj() {return this->adj;}
    void addChave(int chave) {this->chave.push_back(chave);};
    vector<int> getChave() {return this->chave;}
};

class grafo
{
private:
    vector<vertice> sala;
public:

    void addSala(int id);
    vector<vertice> &getSala() {return this->sala;}
    vertice &getUnicaSala(int pos) {return this->sala[pos - 1];}
    void clear(){sala.clear();}
};

void grafo::addSala(int id)
{
    sala.push_back(vertice(id));
}

void BFS (grafo &G, int s )
{
    queue<int> fila {};
    int num;
    //initialize single source
    G.getUnicaSala(s).setCor(1); //configurações da primeira sala
    fila.push(G.getUnicaSala(s).getNumero()); //coloca a sala 1 na fila

    while(!fila.empty())
    {
        num = fila.front();
        fila.pop();
        //abrir a sala que o 1 tem a chave
        for(int c: G.getUnicaSala(num).getChave()) //percorre o vetor de chaves e abre
        {
            G.getUnicaSala(c).setAberta(true);
        }
        for(int i : G.getUnicaSala(num).getAdj()) //percorre o numero dos adjacentes
        {
            if(G.getUnicaSala(i).getCor() == 0) //se a sala for branca
            {
                G.getUnicaSala(i).setCor(1); //atualiza a cor pra cinza
                if(G.getUnicaSala(i).getAberta())
                {
                    fila.push(G.getUnicaSala(i).getNumero());//se ela estiver aberta é colocada na fila para ser explorada
                }else{
                    G.getUnicaSala(i).setFlag(true); //caso estiver trancada é marcada como explorada, para possivelmente voltar
                }
            }
        }
        G.getUnicaSala(num).setCor(2); //apos explorar todos adjacentes coloca a cor como preta
    }

    //se saiu da fila vou percorrer novamente todos os vertices para ver se alguma sala marcada com a flag esta aberta
    for(vertice& v : G.getSala())
    {
        if(v.getFlag() == true && v.getAberta() == true) //sala aberta e exploravel
        {
            v.setFlag(false); //retorno a flag para false para controle
            BFS(G, v.getNumero()); //chamo recursivamente o BFS
        }
    }
}

bool verifica (grafo G) //função que verifica se todas as salas foram abertas e seus adjacentes explorados
{
    for(vertice v : G.getSala())
    {
        if(!v.getAberta()|| v.getCor() != 2)
        {
            return false;
        }
    }
    return true;
}

int main()
{
    unsigned int m,n,x,y,z;
    int skey;
    grafo G;

    while (cin >> n >> m)
    {
        //configuração da sala
        for(unsigned int i = 0; i < n; i++)
        {
            G.addSala(i+1); // adiciona todas as salas com respectivo id
        }
        G.getUnicaSala(1).setAberta(true); //abre sala 1

        //adiciona a lista de adjacentes
        for(unsigned int i = 0; i < m; i++)
        {
            cin >> x >> y;
            G.getUnicaSala(x).addAdj(y);
            G.getUnicaSala(y).addAdj(x);
        }

        // adiciona as chaves a cada sala
        for(unsigned int i = 2; i <= n; i++)
        {
            cin >> skey;
            G.getUnicaSala(skey).addChave(i);
            //G.sala[skey - 1].addChave(i);
        }

        BFS(G, 1); //BFS para a sala 1

        if(verifica(G)) //formata a resposta
        {
            cout << "sim" << endl;
        }
        else
        {
            cout << "nao" << endl;
        }
        G.clear();
    }
    return 0;
}
