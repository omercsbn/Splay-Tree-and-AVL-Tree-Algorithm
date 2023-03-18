# Splay Tree and AVL Tree Algorithm

Bu proje, bir metin dosyasındaki karakterleri okuyarak AVL ve Splay ağaçlarına ekleyen ve her karakterin kaç kez tekrarlandığını hesaplayan bir C programıdır.

# Kullanım
Programın çalıştırılması için bir metin dosyası ve iki argüman gereklidir:

```
./program dosya_adi.txt AVL/Splay
```

**dosya_adi.txt**: Programın karakterleri okuyacağı metin dosyasının adıdır.  
**AVL/Splay**: Programın AVL veya Splay ağacını kullanıp kullanmayacağıdır.

# Çıktı
Program, metindeki her karakterin ağaçlara eklenmesi ve kaç kez tekrarlandığının hesaplanması için çalıştırıldığında, aşağıdaki çıktıları verir:

- Karakterlerin kaç kez tekrarlandığı.
- AVL veya Splay ağaçlarının preorder gezinme sıraları.
- Rotate sayıları, kaç adımda dengelendiği.

# Örnek Çıktı

```
./program sample.txt AVL
```
```
a : 8
b : 4
c : 2
d : 6
e : 10
f : 1
g : 2
h : 5
i : 7
j : 3
AVL preorder traversal: e a d b c h g i f j 
```


# Programın Yapısı
Program, aşağıdaki bölümlerden oluşur:

- main fonksiyonu: Metin dosyasını okur, AVL veya Splay ağacını oluşturur ve karakterleri ağaçlara ekler.
- AVLNode yapısı: AVL ağacı için bir düğüm yapısıdır. Her düğüm bir karakter ve tekrar sayısı içerir, ayrıca sol ve sağ alt ağaçlarına işaret eden işaretçileri vardır.
- SplayNode yapısı: Splay ağacı için bir düğüm yapısıdır. Her düğüm bir karakter ve tekrar sayısı içerir, ayrıca sol, sağ ve üst ağaçlarına işaret eden işaretçileri vardır.
- avl_create_node fonksiyonu: AVL düğümü oluşturur.
- splay_create_node fonksiyonu: Splay düğümü oluşturur.
- avl_insert fonksiyonu: AVL ağacına bir karakter düğümü ekler ve AVL ağacını dengelemek için gerekli döndürmeleri yapar.
- splay_splay fonksiyonu: Splay ağacındaki bir düğümü yukarıya doğru hareket ettirir.
- avl_preorder fonksiyonu: AVL ağacının preorder gezinme sırasını yazdırır.
- splay_preorder fonksiyonu: Splay ağacının preorder gezinme sırasını yazdırır.
