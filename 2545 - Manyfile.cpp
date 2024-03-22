//==================================================================//
/// Grafos para a disciplina de Tecnicas de Programacao            ///
/// IFMG Bambui - Engenharia de Computacao 2023                    ///
/// Alunos:                                                        ///
/// 0034077 - Felipe de Freitas Silva                              ///
/// 0036703 - Jean Gustavo Ferreira Rezende                        ///
//==================================================================//
/*
esse foi o primeiro algoritmo a ser aceito e o primeiro que utilizamos vetor
de adjacentes para cada vertice.
Nesse é utilizado ordenação topologica, que consiste em colocar em uma fila
todos os arquivos(vertices) sem dependencia e "compilar" esses codigos
então retiramos esse arquivo ja pronto da lista de dependencias que ele estiver.
A variavel quantDependencias foi essencial pra isso, ja que não era necessario remover
do vetor toda vez, so conferir se fazia parte.
A cada vez que era retirada a dependencia de um vertice o tempo de execução dela mudava
esse tempo é o maior valor entre a dependencia que foi retirada, e o seu proprio tempo + 1.
E sempre que o vertice tem a lista de dependencias zerada, ele é incluido na fila, para
remove-lo de lista de dependencia dos outros.
Apos isso a função verifica tempo percorre os vertices, se ainda tem alguma dependencia
é porque é impossivel compilar o arquivo, então retorna -1, mas se não tem dependencias
compara e pega o maior valor entre os tempos de compilação.
*/

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class vertice
{
private:
    int id = 0; //identificador
    int tempo = 0; //soma dos tempos de compilação
    int quantDependencias = 0; //quantidade de dependencias
    vector<int> dependencias = {}; //vetor de dependencias lista de adj

public:
    vertice(int id) {this->id = id;};
    void setId(int id) {this->id = id;}
    int getId() {return this->id;}
    void setTempo(int tempo) {this->tempo = tempo;}
    int getTempo() {return this->tempo;}
    void setQuantDependencias(int n) {this->quantDependencias = n;}
    int getQuantDependencias() {return this->quantDependencias;}
    void setDependencia(int id){dependencias.push_back(id);}
    vector<int> const &getDependencias() const {return this->dependencias;}
};

class grafo
{
private:
    vector<vertice> arquivo = {};
public:
    void addArquivo(int id);
    vector<vertice> &getArquivo() {return this->arquivo;}
    vertice &getUnicoArquivo(int pos) {return this->arquivo[pos];}
    void clear() {this->arquivo.clear();}
};
void grafo::addArquivo(int id)
{
    vertice aux(id);
    arquivo.push_back(aux);
}

void topologicalSort(grafo &G)
{
    queue<int> fila;

    //se nao tem dependencia add a fila
    for(vertice v: G.getArquivo())
    {
        if( v.getQuantDependencias() == 0)
        {
            fila.push(v.getId()); //coloca o id do arquivo na fila
        }
    }
    //percorre a lista enquanto não esvaziar
    while(!fila.empty())
    {
        int num = fila.front(); //copia o primeiro elemento da fila
        fila.pop(); //e remove

        for(vertice& v: G.getArquivo()) //percorre o vetor de vertices
        {
            for(int d: v.getDependencias()) //percorre as dependencias de cada vertice
            {
                if(d == num) //se a dependencia corresponde ao num tirado da fila
                {
                    v.setQuantDependencias(v.getQuantDependencias() - 1); //diminui em 1 a quantidade de dependencias
                    v.setTempo(max(v.getTempo(), (G.getUnicoArquivo(num - 1).getTempo()) + 1)); //e atualiza o tempo com o tempo da dependencia que foi removida

                    if(v.getQuantDependencias() == 0) //se a quantidade de dependencias zerar coloca na fila
                    {
                        fila.push(v.getId());
                    }
                }
            }
        }
    }
}

int verificaTempo(grafo& G) //percorre os vertices
{
    int tempoTotal = 0;
    for(vertice v: G.getArquivo())
    {
        if(v.getQuantDependencias() != 0) //se algum vertice ainda tem dependencias é impossivel completar compilação
        {
            return -1;
        }
        tempoTotal = max(tempoTotal, v.getTempo()); //tempo = max entre todos os vertices
    }
    return tempoTotal + 1; //+1 referente a primeira execução
}

int main()
{
    grafo G;
    int n, m, y;

    while (cin >> n )
    {
        //vertices
        for(int i = 0; i< n; i++)
        {
            G.addArquivo(i+1);
        }
        //dependencias
        for (int i = 0; i < n; i++)
        {
            cin >> m; //quantidade de arquivos que i depende
            G.getUnicoArquivo(i).setQuantDependencias(m); //coloco a quantidade de dependencias

            for( int j = 0; j < m; j++) //for para incluir todas as dependencias
            {
                cin >> y;
                G.getUnicoArquivo(i).setDependencia(y); //coloco a dependencia
            }
        }
        topologicalSort(G); //chamo a ordenação topologica
        cout <<  verificaTempo(G) << endl; //o verificaTempo retorna a resposta
        //cout << endl;

        //limpeza de vetores
        G.clear();
    }
    return 0;
}
