#include "main.h"

int MAXSIZE = 20;


#define a_small 97
#define z_small 122
#define A_big 65
#define Z_big 90


bool is_Uppercase(char a){
	int temp = (int)a;
	return temp>= A_big && temp <= Z_big;
}
bool is_Lowercase(char a){
	int temp = (int)a;
	return temp>=a_small && temp <=z_small;
}


//class def
class Huff_Node{
	//for Huff tree to accessed all
	friend class Huff_Tree;

	private:
		char key;
		int frequency;
		bool is_CHAR;
		Huff_Node* left;
		Huff_Node* right;
	public:
		//for key Node
		Huff_Node(char key, Huff_Node* left = nullptr, Huff_Node* right = nullptr){
			this->key = key;
			this->frequency = 1;
			this->is_CHAR = true;
			this->left = left;
			this->right = right;
		}
		//for weight Node

		Huff_Node(int frequency, Huff_Node* left = nullptr, Huff_Node* right = nullptr){
			this->key = ' ';
			this->frequency = frequency;
			this->left = left;
			this->right = right;
			this->is_CHAR = false;
		}




		char get_key(){
			return key;
		}
		int get_Frequency(){
			return frequency;
		}
		bool get_is_CHAR(){
			return is_CHAR;
		}

		void change_key(char key){
			this->key = key;
		}

		void operator += (Huff_Node* newNode){
			this->frequency += newNode->get_Frequency();
		}
		void operator += (int frequency){
			this->frequency += frequency; 
		}
		
		

}; 

class Huff_Tree{
	private:
		Huff_Node* root;
		map<char,string> Huffman_Code;

	public:	

		//rotating

		static Huff_Node* rotate_Left( Huff_Node* Node, int &num){
			Huff_Node* ans = Node->right;

			Node->right = ans-> left;

			ans->left = Node;
			num++;
			return ans;

		}
		
		static Huff_Node* rotate_Right(Huff_Node* Node,int &num){
			Huff_Node* ans = Node->left;

			Node->left = ans->right;

			ans->right = Node;

			num++;
			return ans;
		}
		
		static Huff_Node* left_left(Huff_Node* Node,int& num){
			return rotate_Right(Node,num); 
		}
		static Huff_Node* left_right(Huff_Node* Node,int& num){
			Node->left = rotate_Left(Node->left,num);
			if(num==3) return Node;
			return rotate_Right(Node,num);


		}
		static Huff_Node* right_right(Huff_Node* Node,int& num){
			return rotate_Left(Node,num);

		}
		static Huff_Node* right_left(Huff_Node* Node,int& num){
			Node->right = rotate_Right(Node->right,num);
			if(num==3) return Node;
			return rotate_Left(Node,num);
		}

		static void Count_frequency(string s,vector<Huff_Node*> &arr){

			for(unsigned i=0;i<s.length(); i++){
				//add to arr
				bool is_new = true;
				for(unsigned j=0; j<arr.size();j++){
					if(arr[j]->get_key() ==  s[i]){
						is_new = false;
						(*arr[j])+=1;
						break;
					}
					
				}

				if(is_new){
					arr.push_back(new Huff_Node(s[i]));
				}
				
			}
		}

		static void Ceasar(vector<Huff_Node*> &arr, map<char, char> &map){

			vector<Huff_Node*> ans;

			while(arr.size()>0){
				//extract the char
				char old_char = arr[0]->get_key();
				

				char newchar;

				if(is_Uppercase(old_char)) newchar = (char) ('A' + (((int)old_char - 'A' + arr[0]->get_Frequency()) % 26) );
				else newchar = (char) ('a' + (((int)old_char - 'a' + arr[0]->get_Frequency()) % 26));
				
				Huff_Node* temp = arr[0];
				arr.erase(arr.begin());
				temp->change_key(newchar);
				
				//loop to find duplicate
				bool isNew = true;
				for(unsigned i=0;i< ans.size();i++){
					if(ans[i]->get_key() == newchar){
						*(ans[i]) += temp;
						isNew = false;
						delete temp;
						break;
					}
				}

				if(isNew){
					map[old_char] = newchar;
					ans.push_back(temp);
				}

			}

			arr=ans;
		}

		

		static bool Compare_By_Frequency(Huff_Node* a, Huff_Node* b){
			if(a->get_Frequency()<b->get_Frequency()) return true;
			
			return is_Lowercase(a->get_key()) && is_Uppercase(b->get_key());
		}
		static void Sort_By_Frequency(vector<Huff_Node*> &arr){
			sort(arr.begin(),arr.end(), Compare_By_Frequency);

		}




		//balancing factor
		static int get_Height(Huff_Node* Node){
			if(Node == nullptr) return 0;

			return max(get_Height(Node->left), get_Height(Node->right))+1;
		}
		static int get_Ballancing_factor(Huff_Node* Node){
			return -get_Height(Node->left) + get_Height(Node->right);
		}

	public:
		Huff_Tree(){
			root = nullptr;
		}
		

		Huff_Node* ballancing(Huff_Node* Node,int &num){
			//pre order
			if(num==3) return Node;
			if(Node == nullptr) return Node;

			int ballancing_Factor = get_Ballancing_factor(Node);

			if(ballancing_Factor < -1){
				//left case
				//check if left left or right right
				int temp_ballancing_factor =get_Ballancing_factor(Node->left);
				if(temp_ballancing_factor <= 0){
					//left left
					Node = left_left(Node,num);
				}
				else{
					//left right 
					Node = left_right(Node,num); 
				}
				
			}
			else if(ballancing_Factor >1){
				//right case
				int temp_ballancing_factor = get_Ballancing_factor(Node->right);
				if(temp_ballancing_factor <=0){
					//right right
					Node = right_right(Node,num);
				}
				else{
					//right let
					Node = right_left(Node,num);
				}
			}
			else{/* khong lam gi ca */ }
			Node->left = ballancing(Node->left,num);
			Node->right =ballancing(Node->right,num);
			return Node;

		}


		void buldHuff(vector<Huff_Node*> arr){	
			while(arr.size()>1){
				//take the first 2 Node out of the arr
				Huff_Node* node1 = arr[0];
				Huff_Node* node2 = arr[1];
				arr.erase(arr.begin());
				arr.erase(arr.begin());
				//create a new tree
				Huff_Node* new_Node = new Huff_Node(node1->get_Frequency()+node2->get_Frequency(), node1,node2);
				//balancing
				int num = 0;
				new_Node = ballancing(new_Node,num);
				//add back to the list
				arr.push_back(new_Node);
				//sort
				for(int i=(int)arr.size()-1;i > 0 && arr[i-1]->get_Frequency() > arr[i]->get_Frequency();i--){
					//swap
					Huff_Node* temp = arr[i];
					arr[i] = arr[i-1];
					arr[i-1] = temp;
				}
			}
			Huff_Node* ans = arr[0];
			arr.clear();
			this->root = ans;

		}

		Huff_Node* get_Root(){
			return this->root;
		}

		static void Print(Huff_Node* node){
			if(node == nullptr) return;
			
			Print(node->left);
			if(node->get_is_CHAR()) cout<<node->get_key()<<endl;
			else cout<<node->get_Frequency()<<endl;
			Print(node->right);
		}
		

		void decodeTheMap(string path,Huff_Node* node){
			if(node == nullptr) return;
			if(node->get_is_CHAR()) Huffman_Code[node->get_key()] = path;
			decodeTheMap(path+'0',node->left);
			decodeTheMap(path+'1',node->right);

		}
		
		int String_to_binary(string binary){
			long long ans =0 ;
			int size = binary.length()-1;
			
			for(int count = 0; size - count >=0 && count<10; count++){
				if(binary[size-count]=='1') ans += pow(2,count);
				
			}
			return (int) ans % MAXSIZE;
		}

		int extractResult(string id){
			string temp_String = "";
			int count =(int) id.length()-1;
			while(temp_String.length()<10 && count>=1){
				temp_String = Huffman_Code[id[count]] + temp_String;
				count--;
			}

			return String_to_binary(temp_String);
		}


};



class restaurant{
	private:
		


	public:

	
};



void simulate(string filename)
{
	//read file
	ifstream file(filename);
	restaurant a();
	
	string line;

	while(getline(file,line)){
		stringstream ss(line);
		string command;
		ss>>command;
		if(command == "MAXSIZE"){
			ss>>MAXSIZE;
			cout<<MAXSIZE;
		}
		else if(command == "LAPSE"){
			//LAPSE <NAME>
			string name;
			ss>>name;
			cout<<name<<endl;

		}
		else if(command == "KOKUSEN"){
			//KOKUSEN
			cout<<command;

		}
		else if(command == "KEITEIKEN"){
			//KEITEIKEN <NUM>
			int num;
			ss>>num;


		}
		else if(command == "HAND"){
			//HAND
			

		}
		else if(command == "LIMITLESS"){
			//LIMITLESS <NUM>
			int num;
			ss>>num;

		}
		else if(command == "CLEAVE"){
			//CLEAVE <NUM>
			int num;
			ss>>num;


		}
		


	}

	
	

	

	
	

	
	return;
}