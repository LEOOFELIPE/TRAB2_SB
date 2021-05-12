#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class linha
{   
    public:
    string indice, conteudo;
    linha(string ind, string cont)
    {
        indice = ind;
        conteudo = cont;
    }
}; 

class definicao
{   
    public:
    int valor;
    string nome;
    definicao(string name, int value)
    {   
        nome = name;
        valor = value;
        
    }
};

class uso
{   
    public:
    int endereco;
    string nome;
    uso(string name, int end)
    {   
        nome = name;
        endereco = end;
        
    }
};

vector <string> split (string linha, string delimitador)
{
    int inicio_linha = 0;
    int pos_linha = 0;
    int size_delimitador = delimitador.length();
    vector <string> termo_linha;
    string linha_split = linha;
    string aux;

    while((pos_linha = linha_split.find(delimitador, inicio_linha)) != string::npos)
    {
        aux = linha_split.substr(inicio_linha, pos_linha - inicio_linha);
        inicio_linha = pos_linha + delimitador.length(); //retira o limitador
        termo_linha.push_back(aux); //salva termo a termo
    }
 
    termo_linha.push_back(linha_split.substr(inicio_linha)); 
    return termo_linha;
}
string retira_vazio(string linha)
{
    // Declara nova string
    string linhaNova;
    // Percorre string elemento a elemento
    if(linha.size() == 0)
    {
        return linha;
    }
    for (size_t i = 0; i < linha.length(); i++)
    {
        if(linha[i] == ' ' && linha[i+1] == ' ')
        {
          continue;
        }
        else{
            linhaNova.push_back(linha[i]);
        }
    }
    //Retirar o ultimo espaço, caso tenha
    if(linhaNova[linhaNova.size()-1] == ' '){
        linhaNova = linhaNova.substr(0, linhaNova.size()-1);
    }
    
    if(linhaNova.size() != 0)
    {
        if(linhaNova[0] == ' ')
        {
            linhaNova = linhaNova.substr(1, linhaNova.size()-1);
        }
    }
    return linhaNova;    
}

vector <linha*> vetor_linhas(vector <linha*> vetorLinhas, string o_file)
 {
    fstream arquivo_asmIN;
    string linhaBin;
    int i = 1;
    //abre o arquivo de entrada
    arquivo_asmIN.open(o_file);
    //verifica para acessar o arquivo de entrada
    if(arquivo_asmIN.is_open())
    {   
        //acessa o arquivo de entrada para inserir no vetorLinhas 
        while(getline(arquivo_asmIN, linhaBin))
        {   
            //vetorLinhas.push_back(linha);
            vector <string> aux = split(linhaBin,",");
            if(aux.size() != 0)
            {   
                linha* line = new linha(aux[0],aux[1]);
                vetorLinhas.push_back(line);
            }
            i++;
        }
        //fechar o arquivo
        arquivo_asmIN.close();
        
    }else{
        cout << "N foi possivel abrir o seu arquivo" << endl;
    }
    /*for (size_t i = 0; i < vetorLinhas.size(); i++)
    {
        cout << vetorLinhas[i] -> indice << " --- " << vetorLinhas[i] -> conteudo <<endl;
    }
    cout << "------------------------------------------------------" << endl;*/

    return vetorLinhas;
}

vector <string> vetor_bitmap( vector <linha*> vetorLinhas)
{   
    string bitmap;
    for (size_t i = 0; i < vetorLinhas.size(); i++)
    {
        if(vetorLinhas[i] -> indice == "R")
        {
            bitmap = vetorLinhas[i] -> conteudo;
            
        }
    }
    vector <string> vetor_bitmap = split(bitmap, " ");
    
    return vetor_bitmap;
}

vector <string> vetor_obj( vector <linha*> vetorLinhas)
{   
    string obj;
    for (size_t i = 0; i < vetorLinhas.size(); i++)
    {
        if(vetorLinhas[i] -> indice == "T")
        {
            obj = vetorLinhas[i] -> conteudo;
            
        }
    }
    vector <string> vetor_obj = split(obj, " ");
    
    return vetor_obj;
}

void cria_arquivo_obj(string asm_name, vector <string> OPC_inst)
{
    fstream arquivo_objeto;

    arquivo_objeto.open(asm_name + ".obj", ios::out);
    if(arquivo_objeto.is_open())
    {   
        for (size_t i = 0; i < OPC_inst.size(); i++)
        {
            arquivo_objeto << OPC_inst[i] << " ";    
        }
    }
    else{
        cout << "N foi possivel abrir o seu arquivo" << endl;
    }
}
int pega_fator_de_correcao(vector <linha*> vetorLinhas)
{   
    int fator_correcao = 0;
    for (size_t i = 0; i < vetorLinhas.size(); i++)
    {
        if(vetorLinhas[i] -> indice == "L")
        {
            fator_correcao = stoi(vetorLinhas[i] -> conteudo);
            
        }
    }
    return fator_correcao;
}

vector <string> corrige_pendencias(vector <string> vetorObj, vector <string> vetorBitmap, int FC)
{
    for (size_t i = 0; i < vetorBitmap.size(); i++)
    {
        if(vetorBitmap[i] == "1")
        {
            int vetor_obj = stoi(vetorObj[i]);
            vetor_obj += FC;
            vetorObj[i] = to_string(vetor_obj);
        }
    }
    return vetorObj;
}

vector <definicao*> geraTGD(vector <linha*> vetor_mod1, vector <linha*> vetor_mod2, int FC)
{   vector <definicao*> tabelaGeral_definicoes = {};

    for (size_t i = 0; i < vetor_mod1.size(); i++)
    {
        if(vetor_mod1[i] -> indice == "D")
        {   
            vector <string> aux = split(vetor_mod1[i] -> conteudo, " "); 
            
            definicao* new_definicao = new definicao(aux[0], stoi(aux[1]));
            tabelaGeral_definicoes.push_back(new_definicao);
        }
    }

    for (size_t i = 0; i < vetor_mod2.size(); i++)
    {
        if(vetor_mod2[i] -> indice == "D")
        {   
            vector <string> aux = split(vetor_mod2[i] -> conteudo, " "); 
            
            definicao* new_definicao = new definicao(aux[0], stoi(aux[1]) + FC);
            tabelaGeral_definicoes.push_back(new_definicao);
        }
    }
    
    /*for (size_t i = 0; i < tabelaGeral_definicoes.size(); i++)
    {
        cout << tabelaGeral_definicoes[i] -> nome << "--" << tabelaGeral_definicoes[i] -> valor << endl;
    }*/
    
    return tabelaGeral_definicoes;
}
vector <definicao*> geraTGD3(vector <linha*> vetor_mod1, vector <linha*> vetor_mod2, vector <linha*> vetor_mod3, int FC, int FC2)
{   vector <definicao*> tabelaGeral_definicoes = {};

    for (size_t i = 0; i < vetor_mod1.size(); i++)
    {
        if(vetor_mod1[i] -> indice == "D")
        {   
            vector <string> aux = split(vetor_mod1[i] -> conteudo, " "); 
            
            definicao* new_definicao = new definicao(aux[0], stoi(aux[1]));
            tabelaGeral_definicoes.push_back(new_definicao);
        }
    }

    for (size_t i = 0; i < vetor_mod2.size(); i++)
    {
        if(vetor_mod2[i] -> indice == "D")
        {   
            vector <string> aux = split(vetor_mod2[i] -> conteudo, " "); 
            
            definicao* new_definicao = new definicao(aux[0], stoi(aux[1]) + FC);
            tabelaGeral_definicoes.push_back(new_definicao);
        }
    }
    for (size_t i = 0; i < vetor_mod3.size(); i++)
    {
        if(vetor_mod3[i] -> indice == "D")
        {   
            vector <string> aux = split(vetor_mod3[i] -> conteudo, " "); 
            
            definicao* new_definicao = new definicao(aux[0], stoi(aux[1]) + FC + FC2);
            tabelaGeral_definicoes.push_back(new_definicao);
        }
    }
    
    /*for (size_t i = 0; i < tabelaGeral_definicoes.size(); i++)
    {
        cout << tabelaGeral_definicoes[i] -> nome << "--" << tabelaGeral_definicoes[i] -> valor << endl;
    }*/
    
    return tabelaGeral_definicoes;
}

vector <uso*> geratableUSO(vector <linha*> vetor_mod)
{   vector <uso*> tabela_uso = {};

    for (size_t i = 0; i < vetor_mod.size(); i++)
    {
        if(vetor_mod[i] -> indice == "U")
        {   
            vector <string> aux = split(vetor_mod[i] -> conteudo, " "); 
            
            uso* new_definicao = new uso(aux[0], stoi(aux[1]));
            tabela_uso.push_back(new_definicao);
        }
    }

    /*for (size_t i = 0; i < tabela_uso.size(); i++)
    {
        cout << tabela_uso[i] -> nome << "--" << tabela_uso[i] -> endereco << endl;
    }
    cout << "-------" << endl;*/
    
    return tabela_uso;
}

vector <string> corrige_referencias_cruzadas(vector<string> cod_obj, vector <uso*> tabela_uso, vector <definicao*> tabela_geral_DEF)
{
    for (size_t i = 0; i < tabela_uso.size(); i++)
    {
        string aux = tabela_uso[i] -> nome;
        int valor_uso = 0;
        for (size_t j = 0; j < tabela_geral_DEF.size(); j++)
        {
            if(aux == tabela_geral_DEF[j] -> nome)
            {
                valor_uso = tabela_geral_DEF[j] -> valor;
            }
        }
        cod_obj[tabela_uso[i] -> endereco] = to_string(valor_uso);        
    }
    /*for (size_t i = 0; i < cod_obj.size(); i++)
    {
        cout << cod_obj[i] << " ";
    }*/
    
    return cod_obj;
}

void cria_arquivo_obj3(string asm_name, vector <string> obj1, vector <string> obj2, vector <string> obj3)
{
    fstream arquivo_objeto;
    string objeto_name = asm_name;

    arquivo_objeto.open(objeto_name+".obj", ios::out);
    if(arquivo_objeto.is_open())
    {   
        for (size_t i = 0; i < obj1.size(); i++)
        {
            if(i == obj1.size()-1)
            {
                arquivo_objeto << obj1[i];
            }
            else{
                arquivo_objeto << obj1[i] << " ";
            }
        }
        for (size_t i = 0; i < obj2.size(); i++)
        {
            if(i == obj2.size()-1)
            {
                arquivo_objeto << obj2[i];
            }
            else{
                arquivo_objeto << obj2[i] << " ";
            }
        }
        for (size_t i = 0; i < obj3.size(); i++)
        {
            if(i == obj3.size()-1)
            {
                arquivo_objeto << obj3[i];
            }
            else{
                arquivo_objeto << obj3[i] << " ";
            }
        }
        
    }    
    else{
        cout << "N foi possivel abrir o seu arquivo" << endl;
    }
}
void cria_arquivo_obj2(string asm_name, vector <string> obj1, vector <string> obj2)
{
    fstream arquivo_objeto;
    string objeto_name = asm_name;

    arquivo_objeto.open(objeto_name+".obj", ios::out);
    if(arquivo_objeto.is_open())
    {   
        for (size_t i = 0; i < obj1.size(); i++)
        {   
            if(i == obj1.size()-1)
            {
                arquivo_objeto << obj1[i];
            }
            else{
                arquivo_objeto << obj1[i] << " ";
            }
        }
        for (size_t i = 0; i < obj2.size(); i++)
        {
            if(i == obj2.size()-1)
            {
                arquivo_objeto << obj2[i];
            }
            else{
                arquivo_objeto << obj2[i] << " ";
            }
        }    
    }    
    else{
        cout << "N foi possivel abrir o seu arquivo" << endl;
    }
}

int main(int argc, char *argv[])
{   
    
    if(argc == 2)
    {   
        vector <string> vetorObj1 = {};
        vector <string> vetorBitmap1 = {};
        vector <linha*> vetorLinhas1 = {};
        vetorLinhas1 = vetor_linhas(vetorLinhas1, argv[1]);
        vetorBitmap1 = vetor_bitmap(vetorLinhas1);
        vetorObj1 = vetor_obj(vetorLinhas1);
        cria_arquivo_obj(split(argv[1],".")[0], vetorObj1);

    }
    else if(argc == 3)
    {
        vector <string> vetorObj1 = {};
        vector <string> vetorBitmap1 = {};  
        vector <linha*> vetorLinhas1 = {};
        vetorLinhas1 = vetor_linhas(vetorLinhas1, argv[1]);
        vetorBitmap1 = vetor_bitmap(vetorLinhas1);
        vetorObj1 = vetor_obj(vetorLinhas1);
        int tamanho_mod1 = pega_fator_de_correcao(vetorLinhas1);
        vector <string> vetorObj2 = {};
        vector <string> vetorBitmap2 = {};  
        vector <linha*> vetorLinhas2 = {};
        vetorLinhas2 = vetor_linhas(vetorLinhas2, argv[2]);
        vetorBitmap2 = vetor_bitmap(vetorLinhas2);
        vetorObj2 = vetor_obj(vetorLinhas2);
        
        vector <definicao*> tabela_definicoes_geral = geraTGD(vetorLinhas1, vetorLinhas2, tamanho_mod1);
        vector <uso*> tabela_definicoes_uso1 = geratableUSO(vetorLinhas1);
        vector <uso*> tabela_definicoes_uso2 = geratableUSO(vetorLinhas2);
        vetorObj1 = corrige_referencias_cruzadas(vetorObj1, tabela_definicoes_uso1, tabela_definicoes_geral);
        vetorObj2 = corrige_pendencias(vetorObj2, vetorBitmap2, tamanho_mod1);
        vetorObj2 = corrige_referencias_cruzadas(vetorObj2, tabela_definicoes_uso2, tabela_definicoes_geral);
        cria_arquivo_obj2(split(argv[1],".")[0], vetorObj1, vetorObj2);
        /*for (size_t i = 0; i < vetorObj1.size(); i++)
        {    
            cout << vetorObj1[i] << " ";
        }
        cout << endl;
        for (size_t i = 0; i < vetorObj2.size(); i++)
        {    
            cout << vetorObj2[i] << " ";
        }*/
        cout << endl;
    }
    
    else if(argc == 4)
    {
        vector <string> vetorObj1 = {};
        vector <string> vetorBitmap1 = {};  
        vector <linha*> vetorLinhas1 = {};
        vetorLinhas1 = vetor_linhas(vetorLinhas1, argv[1]);
        vetorBitmap1 = vetor_bitmap(vetorLinhas1);
        vetorObj1 = vetor_obj(vetorLinhas1);
        int tamanho_mod1 = pega_fator_de_correcao(vetorLinhas1);
        vector <string> vetorObj2 = {};
        vector <string> vetorBitmap2 = {};  
        vector <linha*> vetorLinhas2 = {};
        vetorLinhas2 = vetor_linhas(vetorLinhas2, argv[2]);
        vetorBitmap2 = vetor_bitmap(vetorLinhas2);
        vetorObj2 = vetor_obj(vetorLinhas2);
        int tamanho_mod2 = pega_fator_de_correcao(vetorLinhas2);
        vector <string> vetorObj3 = {};
        vector <string> vetorBitmap3 = {};  
        vector <linha*> vetorLinhas3 = {};
        vetorLinhas3 = vetor_linhas(vetorLinhas3, argv[3]);
        vetorBitmap3 = vetor_bitmap(vetorLinhas3);
        vetorObj3 = vetor_obj(vetorLinhas3);
        
        vector <definicao*> tabela_definicoes_geral = geraTGD3(vetorLinhas1, vetorLinhas2, vetorLinhas3 ,tamanho_mod1, tamanho_mod2);
        vector <uso*> tabela_definicoes_uso1 = geratableUSO(vetorLinhas1);
        vector <uso*> tabela_definicoes_uso2 = geratableUSO(vetorLinhas2);
        vector <uso*> tabela_definicoes_uso3 = geratableUSO(vetorLinhas3);
        int FC3 = tamanho_mod1 + tamanho_mod2;
        vetorObj1 = corrige_referencias_cruzadas(vetorObj1, tabela_definicoes_uso1, tabela_definicoes_geral);
        vetorObj2 = corrige_pendencias(vetorObj2, vetorBitmap2, tamanho_mod1);
        vetorObj2 = corrige_referencias_cruzadas(vetorObj2, tabela_definicoes_uso2, tabela_definicoes_geral);
        vetorObj3 = corrige_pendencias(vetorObj3, vetorBitmap3, FC3);
        vetorObj3 = corrige_referencias_cruzadas(vetorObj3, tabela_definicoes_uso3, tabela_definicoes_geral);
        cria_arquivo_obj3(split(argv[1],".")[0], vetorObj1, vetorObj2, vetorObj3);
        /*for (size_t i = 0; i < vetorObj1.size(); i++)
        {    
            cout << vetorObj1[i] << " ";
        }
        cout << endl;
        for (size_t i = 0; i < vetorObj2.size(); i++)
        {    
            cout << vetorObj2[i] << " ";
        }
        cout << endl;
        for (size_t i = 0; i < vetorObj3.size(); i++)
        {    
            cout << vetorObj3[i] << " ";
        }*/
        cout << endl;
    }
    else{
        cout << "TEM ALGO MUITO ERRADO, verifique a forma de executar" << endl;
    }
    return 0;
}

