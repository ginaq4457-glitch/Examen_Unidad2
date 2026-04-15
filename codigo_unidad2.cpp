#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// ================= CLASE LIBRO =================
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
    
    void setDisponibles(int d){ 
        disponibles = d; 
        estado = (disponibles > 0) ? "Disponible" : "Agotado";
    }

    void mostrar(){
        cout << "Cod: " << codigo 
             << "\t| Titulo: " << titulo 
             << "\t| Area: " << area 
             << "\t| Disp: " << disponibles 
             << "\t| Est: " << estado << endl;
    }
};

// ================= CLASE PRESTAMO =================
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
    bool isDevuelto(){ return devuelto; }

    void setMulta(double m){ multa=m; }
    void setDevuelto(bool d){ devuelto = d; }

    void mostrar(){
        cout << "Libro: " << codigoLibro 
             << "\t| Cedula: " << cedula 
             << "\t| Multa: $" << multa 
             << "\t| Devuelto: " << (devuelto ? "Si" : "No") 
             << "\t| Reserva: " << (reservado ? "Si" : "No") << endl;
    }
};

// ================= FUNCIONES DE VALIDACION Y LOGICA =================

bool validarCedula(string cedula) {
    if (cedula.length() != 10) return false;
    for (char c : cedula) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// CORRECCION DEL FRAGMENTO DEFECTUOSO
bool prestarLibro(int disponibles, bool bloqueado){
    return (disponibles > 0 && !bloqueado);
}

double calcularMulta(int retraso){
    return retraso * 0.75; 
}

// RECURSIVIDAD: Suma de multas
double totalMultasRecursivo(vector<PrestamoBiblioteca>& p, int i){
    if(i >= (int)p.size()) return 0;
    return p[i].getMulta() + totalMultasRecursivo(p, i+1);
}

// ORDENAMIENTO BURBUJA MANUAL
void ordenarPorMulta(vector<PrestamoBiblioteca>& p){
    int n = p.size();
    for(int i = 0; i < n - 1; i++){
        for(int j = 0; j < n - i - 1; j++){
            if(p[j].getMulta() < p[j+1].getMulta()){
                swap(p[j], p[j+1]);
            }
        }
    }
}

// ================= MODULO AHORCADO CORREGIDO =================

string ocultarPalabra(string palabra, const vector<char>& usadas){
    string resultado = "";
    for(char c : palabra){
        if(c == ' ') { resultado += "  "; continue; }
        bool encontrada = false;
        for(char u : usadas) { if(tolower(u) == tolower(c)) encontrada = true; }
        resultado += (encontrada ? c : '_');
        resultado += " ";
    }
    return resultado;
}

void jugarAhorcado(vector<Libro>& libros, vector<string>& bloqueados) {
    if(libros.empty()){ cout << "Registre libros primero.\n"; return; }
    
    int idx = rand() % libros.size();
    string palabra = libros[idx].getTitulo();
    vector<char> usadas;
    int intentos = 5;

    cout << "\n--- AHORCADO BIBLIOTECARIO ---\n";
    cout << "Categoria: " << libros[idx].getArea() << endl;

    while(intentos > 0) {
        string actual = ocultarPalabra(palabra, usadas);
        cout << "\nPalabra: " << actual << endl;
        
        // CORRECCION: Si ya no hay guiones, ganaste y salimos antes de pedir letra
        if(actual.find('_') == string::npos) {
            cout << "FELICIDADES! Ganaste y evitaste el bloqueo.\n";
            return;
        }

        cout << "Intentos: " << intentos << " | Ingrese letra: ";
        char letra; cin >> letra;
        usadas.push_back(letra);

        if(palabra.find(tolower(letra)) == string::npos && palabra.find(toupper(letra)) == string::npos) {
            intentos--;
            cout << "Letra incorrecta!\n";
        } else {
            cout << "Bien hecho!\n";
        }
    }

    cout << "\nPERDISTE. La palabra era: " << palabra << endl;
    string ci;
    do {
        cout << "Ingrese cedula (10 digitos) para aplicar BLOQUEO: ";
        cin >> ci;
        if(!validarCedula(ci)) cout << "ERROR: Cedula invalida.\n";
    } while(!validarCedula(ci));
    
    bloqueados.push_back(ci);
    cout << "Usuario bloqueado temporalmente por fallar el modulo.\n";
}

// ================= MAIN =================

int main(){
    srand(time(0));
    vector<Libro> inventario;
    vector<PrestamoBiblioteca> registro;
    vector<string> bloqueados;
    int op;

    do{
        cout << "\n--- SISTEMA BIBLIOTECA - UTA ---\n";
        cout << "1. Registrar Libro\n2. Prestamo\n3. Devolucion\n4. Reporte General\n";
        cout << "5. Ordenar por Deuda (Manual)\n6. Total Multas (Recursivo)\n7. MODULO AHORCADO\n8. Salir\nOpcion: ";
        cin >> op;

        if(op==1){
            string c, t, a; int cant;
            cout << "Cod: "; cin >> c; 
            cout << "Titulo: "; cin.ignore(); getline(cin, t);
            cout << "Area: "; getline(cin, a); 
            cout << "Cantidad: "; cin >> cant;
            inventario.push_back(Libro(c, t, a, cant, cant));
        }
        else if(op==2){
            string c_lib, ci;
            cout << "Cod Libro: "; cin >> c_lib; 
            do {
                cout << "Cedula (10 digitos): "; cin >> ci;
            } while(!validarCedula(ci));
            
            bool estaBloqueado = false;
            for(string b : bloqueados) if(b == ci) estaBloqueado = true;

            for(auto &l : inventario){
                if(l.getCodigo() == c_lib){
                    if(prestarLibro(l.getDisponibles(), estaBloqueado)){
                        l.setDisponibles(l.getDisponibles()-1);
                        registro.push_back(PrestamoBiblioteca(c_lib, ci));
                        cout << "Prestamo exitoso!\n";
                    } else {
                        string razon = estaBloqueado ? "USUARIO BLOQUEADO" : "SIN STOCK (RESERVADO)";
                        cout << "Denegado: " << razon << endl;
                        registro.push_back(PrestamoBiblioteca(c_lib, ci, 0, false, true));
                    }
                }
            }
        }
        else if(op==3){
            string ci; int dias;
            cout << "Cedula: "; cin >> ci; 
            cout << "Dias retraso: "; cin >> dias;
            for(auto &p : registro){
                if(p.getCedula() == ci && !p.isDevuelto()){
                    p.setMulta(calcularMulta(dias));
                    p.setDevuelto(true);
                    cout << "Devuelto. Multa generada: $" << p.getMulta() << endl;
                }
            }
        }
        else if(op==4){
            cout << "\n--- INVENTARIO ---\n"; for(auto l : inventario) l.mostrar();
            cout << "\n--- REGISTROS ---\n"; for(auto p : registro) p.mostrar();
        }
        else if(op==5){
            ordenarPorMulta(registro);
            cout << "Datos ordenados por deuda exitosamente.\n";
        }
        else if(op==6){
            cout << "Deuda total acumulada: $" << totalMultasRecursivo(registro, 0) << endl;
        }
        else if(op==7){
            jugarAhorcado(inventario, bloqueados);
        }
    } while(op != 8);

    return 0;
}