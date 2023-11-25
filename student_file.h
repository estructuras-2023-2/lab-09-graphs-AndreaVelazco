#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std;

class ConjuntoUnion {
private:
  unordered_map<string, string> nodoPrincipal;
  unordered_map<string, int> nivel;

public:
  void crearGrupo(const string& elemento) {
    nodoPrincipal[elemento] = elemento;
    nivel[elemento] = 0;
  }

  string encontrar(const string& elemento) {
    if (nodoPrincipal[elemento] != elemento)
      nodoPrincipal[elemento] = encontrar(nodoPrincipal[elemento]);
    return nodoPrincipal[elemento];
  }

  bool unirGrupos(const string& a, const string& b) {
    string raizA = encontrar(a);
    string raizB = encontrar(b);

    if (raizA == raizB) {
      return false;
    }

    if (nivel[raizA] < nivel[raizB]) {
      nodoPrincipal[raizA] = raizB;
    } else if (nivel[raizA] > nivel[raizB]) {
      nodoPrincipal[raizB] = raizA;
    } else {
      nodoPrincipal[raizB] = raizA;
      nivel[raizA]++;
    }

    return true;
  }
};

struct Camino {
  string id;
  string ciudadInicio;
  string ciudadFin;
  int precio;

  Camino(string i, string c1, string c2, int co = 0) : id(i), ciudadInicio(c1), ciudadFin(c2), precio(co) {}
};

vector<string> divide(const string& cadena, char delimitador) {
  vector<string> resultado;
  size_t inicio = 0;
  size_t fin = cadena.find(delimitador);
  while (fin != string::npos) {
    resultado.push_back(cadena.substr(inicio, fin - inicio));
    inicio = fin + 1;
    fin = cadena.find(delimitador, inicio);
  }
  resultado.push_back(cadena.substr(inicio));
  return resultado;
}

string reconstruyeCaminos(vector<string> listaCaminos) {
  vector<Camino> caminosDeteriorados;
  ConjuntoUnion cu;
  set<string> raices; 

  for (const auto& caminoStr : listaCaminos) {
    auto elementos = divide(caminoStr, ' ');
    string id = elementos[0];
    string ciudadInicio = elementos[1];
    string ciudadFin = elementos[2];

    cu.crearGrupo(ciudadInicio);
    cu.crearGrupo(ciudadFin);
    raices.insert(cu.encontrar(ciudadInicio));
    raices.insert(cu.encontrar(ciudadFin));
  }

  for (const auto& caminoStr : listaCaminos) {
    auto elementos = divide(caminoStr, ' ');
    string id = elementos[0];
    string ciudadInicio = elementos[1];
    string ciudadFin = elementos[2];
    int precio = (elementos.size() > 3) ? stoi(elementos[3]) : 0;

    if (precio == 0) {
      cu.unirGrupos(ciudadInicio, ciudadFin);
    } else {
      caminosDeteriorados.emplace_back(id, ciudadInicio, ciudadFin, precio);
    }
  }

  sort(caminosDeteriorados.begin(), caminosDeteriorados.end(), [](const Camino& a, const Camino& b) {
    if (a.precio != b.precio) {
      return a.precio < b.precio;
    }
    return a.id < b.id;
  });

  set<string> caminosSeleccionados;

  for (const auto& camino : caminosDeteriorados) {
    if (cu.encontrar(camino.ciudadInicio) != cu.encontrar(camino.ciudadFin)) {
      cu.unirGrupos(camino.ciudadInicio, camino.ciudadFin);
      caminosSeleccionados.insert(camino.id);
    }
  }

  if (raices.size() == 1) {
    string resultado;
    for (const auto& id : caminosSeleccionados) {
      resultado += id + " ";
    }
    return resultado.empty() ? "" : resultado.substr(0, resultado.length() - 1);
  } else {
    return "IMPOSIBLE";
  }
}
