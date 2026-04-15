#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

//  CLASE LIBRO 
class Libro {
private:
    string codigo, titulo, area, estado;
    int ejemplares, disponibles;

public:
    Libro(string c="", string t="", string a="", int e=0, int d=0, string es="Disponible") {
        codigo=c; titulo=t; area=a; ejemplares=e; disponibles=d; estado=es;
    }

    string getCodigo(){ return codigo; }
    string getTitulo(){ return titulo; }
    string getArea(){ return area; }
    int getDisponibles(){ return disponibles; }
    int getDemanda(){ return ejemplares - disponibles; }

    void setDisponibles(int d){ disponibles=d; }

    void mostrar(){
        cout<<"Codigo: "<<codigo
            <<" | Titulo: "<<titulo
            <<" | Area: "<<area
            <<" | Disponibles: "<<disponibles
            <<" | Estado: "<<estado<<endl;
    }
};

// Aplicamos otra clase llamada prestamo
class PrestamoBiblioteca {
private:
    string codigoLibro, cedula;
    double multa;
    bool devuelto, reservado;

public:
    PrestamoBiblioteca(string c="", string ce="", double m=0, bool d=false, bool r=false){
        codigoLibro=c; cedula=ce; multa=m; devuelto=d; reservado=r;
    }

    string getCedula(){ return cedula; }
    double getMulta(){ return multa; }

    void setMulta(double m){ multa=m; }
    void setDevuelto(bool d){ devuelto=d; }

    void mostrar(){
        cout<<"Libro: "<<codigoLibro
            <<" | Cedula: "<<cedula
            <<" | Multa: $"<<multa
            <<" | Devuelto: "<<(devuelto?"Si":"No")
            <<" | Reserva: "<<(reservado?"Si":"No")<<endl;
    }
};

// Aplicamos las FUNCIONES

bool prestarLibro(int disponibles, bool bloqueado){
    return (disponibles > 0 && !bloqueado);
}

double calcularMulta(int retraso){
    return retraso * 0.75;
}

// usamos RECURSIVIDAD
double totalMultas(vector<PrestamoBiblioteca>& p, int i){
    if(i >= p.size()) return 0;
    return p[i].getMulta() + totalMultas(p, i+1);
}
// Aplicamos un metodo  BURBUJA
void ordenarMultas(vector<PrestamoBiblioteca>& p){
    for(int i=0;i<p.size();i++)
        for(int j=0;j<p.size()-1;j++)
            if(p[j].getMulta() < p[j+1].getMulta())
                swap(p[j],p[j+1]);
}

// opción para buscar
void buscarLibro(vector<Libro>& l){
    int op;
    cout<<"1. Codigo   2. Titulo: ";
    cin>>op;

    if(op==1){
        string c;
        cout<<"Codigo: ";
        cin>>c;
        for(auto x:l) if(x.getCodigo()==c) x.mostrar();
    }else{
        string t;
        cout<<"Titulo: ";
        cin>>t;
        for(auto x:l) if(x.getTitulo()==t) x.mostrar();
    }
}

// opción de historial
void historialUsuario(vector<PrestamoBiblioteca>& p){
    string c;
    cout<<"Cedula: ";
    cin>>c;
    for(auto x:p) if(x.getCedula()==c) x.mostrar();
}

// juego del ahorcado

string ocultar(string palabra, vector<char> usadas){
    string r="";
    for(char c:palabra){
        bool ok=false;
        for(char u:usadas) if(u==c) ok=true;

        if(ok) r+=c;
        else r+='_';

        r+=' ';
    }
    return r;
}

void ahorcado(vector<Libro>& libros){
    if(libros.empty()){
        cout<<"No hay libros\n";
        return;
    }

    int dificultad;
    cout<<"1. Facil (titulos)\n2. Dificil (terminos)\nOpcion: ";
    cin>>dificultad;

    vector<string> extra={"multa","reserva","catalogo","sancion"};

    string palabra;
    string categoria;

    if(dificultad==1){
        int i=rand()%libros.size();
        palabra=libros[i].getTitulo();
        categoria=libros[i].getArea();
    }else{
        int i=rand()%extra.size();
        palabra=extra[i];
        categoria="Terminos bibliotecarios";
    }

    vector<char> usadas;
    int intentos=6;
    int puntaje=0;

    cout<<"\nCategoria: "<<categoria<<endl;

    while(intentos>0){
        cout<<"\nPalabra: "<<ocultar(palabra,usadas)<<endl;
        cout<<"Intentos: "<<intentos<<endl;
        cout<<"Letra: ";

        char l;
        cin>>l;

        usadas.push_back(l);

        bool acierto=false;
        for(char c:palabra){
            if(c==l) acierto=true;
        }

        if(acierto){
            cout<<"Correcto\n";
            puntaje+=5;
        }else{
            cout<<"Incorrecto\n";
            intentos--;
            puntaje-=2;
            if(puntaje<0) puntaje=0;
        }

        if(ocultar(palabra,usadas).find('_')==string::npos){
            cout<<"\nGANASTE\n";
            break;
        }
    }

    if(intentos==0){
        cout<<"\nPERDISTE. La palabra era: "<<palabra<<endl;
    }

    cout<<"Puntaje final: "<<puntaje<<endl;
}

// uso del main
int main(){
    srand(time(0));

    vector<Libro> libros;
    vector<PrestamoBiblioteca> prestamos;

    int op;

    do{
        cout<<"\n===== SISTEMA BIBLIOTECA =====\n";
        cout<<"1. Registrar libro\n";
        cout<<"2. Prestamo\n";
        cout<<"3. Devolucion\n";
        cout<<"4. Mostrar datos\n";
        cout<<"5. Buscar\n";
        cout<<"6. Reportes\n";
        cout<<"7. Ahorcado\n";
        cout<<"8. Salir\n";
        cout<<"Opcion: ";
        cin>>op;

        if(op==1){
            string c,t,a;
            int e,d;

            cout<<"Codigo: "; cin>>c;
            cout<<"Titulo: "; cin>>t;
            cout<<"Area: "; cin>>a;
            cout<<"Ejemplares: "; cin>>e;
            cout<<"Disponibles: "; cin>>d;

            libros.push_back(Libro(c,t,a,e,d));
        }

        if(op==2){
            string c,ce;
            cout<<"Codigo libro: "; cin>>c;
            cout<<"Cedula: "; cin>>ce;

            for(auto &l:libros){
                if(l.getCodigo()==c){
                    if(prestarLibro(l.getDisponibles(),false)){
                        l.setDisponibles(l.getDisponibles()-1);
                        prestamos.push_back(PrestamoBiblioteca(c,ce));
                        cout<<"Prestamo realizado\n";
                    }else{
                        prestamos.push_back(PrestamoBiblioteca(c,ce,0,false,true));
                        cout<<"No disponible -> Reserva\n";
                    }
                }
            }
        }

        if(op==3){
            string ce;
            int r;

            cout<<"Cedula: "; cin>>ce;
            cout<<"Retraso: "; cin>>r;

            for(auto &p:prestamos){
                if(p.getCedula()==ce){
                    double m=calcularMulta(r);
                    p.setMulta(m);
                    p.setDevuelto(true);
                    cout<<"Multa: "<<m<<endl;
                }
            }
        }

        if(op==4){
            cout<<"\n--- LIBROS ---\n";
            for(auto l:libros) l.mostrar();

            cout<<"\n--- PRESTAMOS ---\n";
            for(auto p:prestamos) p.mostrar();
        }

        if(op==5){
            buscarLibro(libros);
        }

        if(op==6){
            ordenarMultas(prestamos);
            cout<<"Total multas: "<<totalMultas(prestamos,0)<<endl;
            historialUsuario(prestamos);
        }

        if(op==7){
            ahorcado(libros);
        }

    }while(op!=8);

    return 0;
}
