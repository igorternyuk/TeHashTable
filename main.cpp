#include "hashtable.hpp"
#include "hash_utils.hpp"
#include "point.hpp"
#include <iostream>
#include <ctime>
#include <string>

int main()
{
    std::cout << "*********Hash table**********" << std::endl;
    HashTable<int, std::string> hash_table(11, [](const int &key, size_t max){
        return key % max;
    });
    hash_table.insert(179, "Mashkov");
    hash_table.insert(178, "Celentano");
    hash_table.insert(202, "Klitschko");
    hash_table.insert(213, "Valuev");
    hash_table.insert(187, "Garmash");
    hash_table.insert(193, "Baranov");
    hash_table.insert(196, "Lewis");
    hash_table.insert(181, "Merzlikin");
    hash_table.insert(182, "Drozd");
    hash_table.insert(160, "Fedortzov");
    hash_table.insert(183, "Emelianenko");
    hash_table.insert(184, "Tarabukin");
    hash_table.insert(188, "Tiesto");
    hash_table.insert(191, "Virastuk");
    hash_table.insert(177, "RichardGeer");
    hash_table.insert(174, "Baskov");
    hash_table.insert(173, "Bezrukov");
    hash_table.insert(170, "Taratorkina");
    hash_table.insert(11, "Taratorkina");
    hash_table.insert(1100, "Taratorkina");
    hash_table.insert(2000, "Taratorkina");
    hash_table.insert(21, "Taratorkina");
    hash_table.insert(3, "Taratorkina");
    hash_table.insert(5, "Taratorkina");
    hash_table.insert(7, "Taratorkina");

    hash_table.print();
    std::cout << "Size of hash table = " << hash_table.count() << std::endl;
    hash_table.remove(1100);
    hash_table.remove(2000);
    hash_table.remove(3000);
    hash_table.remove(11);
    hash_table.update(3, "Anna Taratorkina");
    hash_table.update(5, "Anna Taratorkina");
    hash_table.update(7, "Anna Taratorkina");
    hash_table.print();
    std::cout << "Size of hash table = " << hash_table.count() << std::endl;

    hash_table[179] = "Vladimir Mashkoff";
    hash_table[213] = "Mykola Valuev";
    hash_table[184] = "Michael Tarabukin";
    hash_table[185] = "Igor Ternyuk";
    std::cout << "ht7 = " << hash_table.get(7) << std::endl;
    std::cout << "ht185 = " << hash_table[185] << std::endl;
    std::cout << "ht666 = " << hash_table[666] << std::endl;
    hash_table.print();
    hash_table[185] = "Igor Olexandrovutch Ternyuk";
    std::cout << "ht185 = " << hash_table[185] << std::endl;


    std::cout << "******* Hash table iterator ********" << std::endl;
    HashTableIterator<int, std::string> cursor(hash_table);
    cursor.reset();

    for(;!cursor.end();cursor.next())
    {
        if(cursor.getData().key == 185)
            cursor.setValue("Ingvarr");
        std::cout << "(" << cursor.getData().key << "," << cursor.getData().value << ")" << std::endl;
    }

    std::cout << "Open adressing hash-table" << std::endl;
    OpenAddressingHashTable<std::string, double> oaht(10, &hash_string2,
                                                      CollisionResolutionMethod::QUADRATIC_PROBING,
                                                      &hash_sedgwick);
    oaht.print();
    oaht.insert("Afanasiev", 80);
    oaht.insert("Baranov", 90);
    oaht.insert("Chechin", 96);
    oaht.insert("Sumskoi", 110);
    oaht.insert("IgorTernyuk", 109.7);
    oaht.insert("Robak", 98);
    oaht.insert("Willy_Khappasalo", 112.65);
    oaht.insert("Emelianenko", 108);
    oaht.insert("KlitschkoW", 110);
    oaht.insert("KlitschkoV", 113);
    oaht.insert("Travnikov", 130);
    oaht.insert("Lewis", 116);
    oaht.insert("Tyson", 109);
    oaht.insert("Valuev", 144);
    oaht.insert("Virastyuk", 157);
    oaht.insert("Savitskas", 175);
    oaht.insert("Isaenko", 100);
    oaht.insert("AntonTe", 68);
    oaht.insert("DerekBanas", 80);
    oaht.insert("Poddubniy", 125);
    oaht.insert("Zhabotinskiy", 154);
    oaht.insert("Vlasov", 135);
    std::cout << "*****" << std::endl;
    oaht.print();
    std::cout << "Fill factor = " << oaht.getFillFactor() << std::endl;

    double weight = 0;
    if(oaht.find("Valuev", weight))
        std::cout << "Valuev's weight is " << weight << " kg." << std::endl;
    oaht.remove("Sumskoi");
    oaht.update("Tyson", 110);
    std::cout << "Weight of Lewis is " << oaht.get("Lewis") << std::endl;
    oaht["Savitskas"] = 120;
    oaht["Varfolomei"] = 60;
    std::cout << "Tyson weight is " << oaht["Tyson"] << std::endl;
    oaht.print();

    return 0;
}
