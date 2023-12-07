#include "main.h"
#include "restaurant.cpp"

int main(int argc, char* argv[]) {
    
    //string fileName = "test.txt";


    //simulate(fileName);

    string abc;
    abc = "asjbfajbfafsasbfjasbfasbfjakbfajbsfjabsfuwejebguawegbewugdsjnfkaldsnfjdksgwekasnjasjfnASFASFASFAAV";
    //cout<<abc;
    vector< Huff_Node*> arr;
    Huff_Tree::Count_frequency(abc,arr);
    map<char,char> map;
    Huff_Tree::Ceasar(arr,map);

    
    //arr.erase(arr.begin());
    for(int i=0;i<(int)arr.size();i++){
        printf("%c: %d\n",arr[i]->get_key(), arr[i]->get_Frequency());
    }
    Huff_Tree::Sort_By_Frequency(arr);
    cout<<"-\n-\n-\n-\n-\n-\n";

    for(int i=0;i<(int)arr.size();i++){
        printf("%c: %d\n",arr[i]->get_key(), arr[i]->get_Frequency());
    }
    cout<<"-\n-\n-\n-\n-\n-\n";
    Huff_Tree* tree = new Huff_Tree();
    tree->buldHuff(arr);
    tree->Print(tree->get_Root());


    return 0;
}
