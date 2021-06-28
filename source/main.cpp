#include <iostream>
#include <map>
#include <vector>

using namespace std;

class cambios {
public:
	int V;
	string campo, valor;	
	cambios() { V = 0; }
	cambios(int vv, const string& a, const string& b){
		V = vv;
		campo = a;
		valor = b;
	}
	cambios(const cambios& inst) {
		V = inst.V;
		campo = inst.campo;
		valor = inst.valor;
	}
	~cambios() {
		valor.clear();
		campo.clear();
	}
};

class node {
public:
	int V;
	map<string, string> T;
	vector<cambios> TC;
	map<int, shared_ptr<node>> TSN;
	node() { V = 0; TSN[V] = nullptr; }
	friend ostream& operator<<(ostream& os, const node& PLNode);
	node(int vv) { V = vv; TSN[V] = nullptr; }
	node(int vv, map<string, string> td){
		T = td;
		V = vv;
		TSN[V] = nullptr;
	}

	void agregar_nodo(int vv, shared_ptr<node> sn = nullptr) {
		TSN[vv] = sn;
	}
	void agregar_cambio(int V, const string& campo, const string& valor) {
		TC.push_back(cambios(V, campo, valor));
	}

	void set_campos() {
		if (T.empty()) {						
			string campo, valor;			
			cout << "campo: ";cin >> campo;
			cout << "valor: ";cin >> valor;
			T[campo] = valor;		
		}
		else			
			cout << "ERROR\n";
	}

	~node() {
		TSN.clear();
		TC.clear();
		T.clear();		
	}
};

ostream& operator<<(ostream& os, const node& PLNode){		
	os << "CAMPO \t VALOR\n";
	os << "----------------\n";
	if (PLNode.T.empty())
		os << "tabla vacia\n";
	for (auto& data : PLNode.T) {
		os << data.first << "\t" << data.second << "\n";
	}	
	for (auto& node : PLNode.TSN) {		
		os << "Version:\t" << node.first << "\n";
	}
	return os;
}

class persistencia {
public:
	shared_ptr<node> H;
	int VV = 0; 
	void find(int V, const string& campo, shared_ptr<node> PP) {
		if (H == nullptr || V >= VV || V < 0){
			cout << "lista vacia\n";
			return;
		}
		shared_ptr<node> P = PP;
		if (P != nullptr) {
			if (V == P->V) {
				if (campo == "->next") {
					if (P->TSN.find(V) != P->TSN.end())	{
						cout << "encontrado:\n";
						cout << *P->TSN[V];
					}else
						cout << "no encontrado\n";
					return;
				}
				if (P->T.find(campo) != H->T.end()){
					cout << "campo encontrado:\n";
					cout << "CAMPO\tVALOR\n";
					cout << "Version: " << V << " " << campo << " : " << P->T[campo] << "\n";
				}
				else {
					cout << "campo no encontrado\n";
				}
				return;
			}
			else {
				if (campo == "->next") {
					if (P->TSN.find(V) != P->TSN.end())	{
						cout << "nodo encontrado:\n";
						cout << *P->TSN[V];
						return;
					}
				}
				vector<cambios>::iterator it;
				if ((it = find_if(P->TC.begin(), P->TC.end(),[&](cambios RR) {return RR.V == V && RR.campo == campo;})) != P->TC.end()){
					cout << "campo encontrado:\n";
					cout << "CAMPO\tVALOR\n";
					cout << "Version: " << V << " " << campo << " : " << (*it).valor << "\n";
					return;
				}
			}
			for (auto& next_node : P->TSN) {
				if (next_node.second != nullptr) {
					find(V, campo, next_node.second);
				}
			}
		}
		else
			cout << "campo no encontrado\n";
		return;
	}

	persistencia() {
		H = nullptr;
	}

	void set_nodo(shared_ptr<node> pointer) {		
		if (H == nullptr && pointer != H) {
			cout << "lista vacia\n";
		}
		if (pointer == H && H == nullptr) {
			VV++;
			H = make_shared<node>(VV);
			pointer = H;
			VV++;
		}
		pointer->set_campos();
	}

	void insert(shared_ptr<node> pointer, const string& campo, const string& valor, shared_ptr<node> newNode = nullptr){
		if (campo.empty()) {
			cout << "campo vacio\n";
			return;
		}
		bool created_head = false;
		if (H == nullptr && pointer == H) {
			VV++;
			created_head = true;
			H = make_shared<node>(VV);
			pointer = H;
		}
		if (campo == "->next")
			(*pointer).TSN[VV] = newNode;
		else
			(*pointer).agregar_cambio(VV, campo, valor);

		if (!created_head)
			VV++;
	}

	void find(int V, const string& campo) {
		if (H == nullptr || V >= VV || V < 0) {
			cout << "ERROR\n";
			return;
		}

		shared_ptr<node> P = H;
		if (P != nullptr) {
			if (V == P->V) {
				if (campo == "->next") {
					if (P->TSN.find(V) != P->TSN.end()){
						cout << "nodo encontrado:\n";
						cout << (*P->TSN[V]) << endl;
					}
					else
						cout << "nodo no encontrado\n";
					return;
				}

				if (P->T.find(campo) != H->T.end())	{
					cout << "campo encontrado:\n";
					cout << "CAMPO\tVALOR\n";
					cout << "Version: " << V << " " << campo << " : " << P->T[campo] << "\n";
				}
				else {
					cout << "campo no encontrado\n";
				}
				return;
			}
			else {
				if (campo == "->next") {
					if (P->TSN.find(V) != P->TSN.end())	{
						cout << "nodo encontrado:\n";
						cout << *P->TSN[V];
						return;
					}
				}

				vector<cambios>::iterator it;
				if ((it = find_if(P->TC.begin(), P->TC.end(),[&](cambios RR) {return RR.V == V && RR.campo == campo;})) != P->TC.end())	{
					cout << "campo encontrado:\n";
					cout << "CAMPO\tVALOR\n";
					cout << "Version" << V << " " << campo << " : " << (*it).valor << "\n";
					return;
				}
			}

			for (auto& next_node : P->TSN) {
				if (next_node.second != nullptr) {
					find(V, campo, next_node.second);
				}
			}
		}
		else
			cout << "campo no encontrado\n";
		system("PAUSE");
		return;
	}
	int VV_actual() const { return VV; }

	~persistencia() {
		H.reset();
	}
};

int main() {
	/*
	* ingresar para que encuentre:
	* >>nombre
	* >>gino
	* >>nombre
	* >>liza
	*/
	persistencia X;
	
	X.set_nodo(X.H);
	cout << (*X.H) << endl;

	shared_ptr<node> node2 = make_shared<node>(X.VV_actual());	
	X.set_nodo(node2);	X.insert(X.H, "->next", "", node2);
	cout << (*X.H->TSN[X.VV_actual() - 1]) << endl;
	cout << " >>>> encontrar \"nombre\" en el primero\n";
	X.find(1, "nombre");//busca por indice
	return 0;
}