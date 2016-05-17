Ce trebuie sa aiba o clasa din librarie?
-Tot ce are nevoie sa fie folosita normal.

vector<string> vs;
Trie t(vs);

Trie.add("qwe");
Trie.add(vs);

Constructori, destructori, operatori.
Se foloseste new sau pointeri fantoma?

Nu?
class T {
  T() = default;
  ~T() = default;
  
  T(const T&) = default;
  T& operator=(const T&) = default;
  
  T(T&&) = default;
  T& operator=(T&&) = default;
}

Da?
*se copiaza valoarea pointerilor.
Daca avem
struct A {
  int *a
} a, b;
a.a = new int;
b = a;
a si b o sa arate spre acelasi int

class T {
public:
  T() = default;
  ~T() = default;
  
  T(const T& rhs) {
    *i = new int(*rhs.i);
    *f = new Foo(*rhs.f);
    *b = new Bar(*rhs.b);
    ...
  }
  T& operator=(const T& rhs) {
    *this = T(rhs);
  }
  
  T(T&&) = default;
  T& operator=(T&&) = default;
  
  int* i;
  Foo* f;
  Bar* b;
}
