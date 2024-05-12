#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

#define VERSION 0.2

//clean the shell and the io
void clean()
{
    system("cls");
    fflush(stdin);
}//clean

//return the levenshtein distance of two strings
int levenshtein_distance(const string& str1, const string& str2) {
  int n = str1.size();
  int m = str2.size();
  int d[n + 1][m + 1];

  if (n == 0) {
    return m;
  }

  if (m == 0) {
    return n;
  }

  for (int i = 0; i <= n; ++i) {
    for (int j = 0; j <= m; ++j) {
      if (i == 0) {
        d[i][j] = j;
      } else if (j == 0) {
        d[i][j] = i;
      } else if (str1[i - 1] == str2[j - 1]) {
        d[i][j] = d[i - 1][j - 1];
      } else {
        d[i][j] = 1 + min(d[i - 1][j], min(d[i][j - 1], d[i - 1][j - 1]));
      }
    }
  }

  return d[n][m];
}//levenshtein_distance

//delete all the blank in the string
string trim(string s)
{
	int index = 0;
	if(!s.empty())
	{
		while( (index = s.find(' ',index)) != string::npos)
		{
			s.erase(index,1);
		}
	}
    return s;
}//trim

//class of the Administrator
class Admin
{
    private:
        string _account;
        string _password;

    public:
        Admin(string accout,string password) : _account(accout),_password(password){}

        //get the Account
        string GetAccount() const {return _account;}

        //get the password
        string GetPassword() const {return _password;}

        //login the admin system
        bool login(string account,string password)
        {
            if((_account.compare(account))==0 && (_password.compare(password))==0)
            {
                cout<<"Admin log in successfully"<<endl;
                return true;
            }else{
                cout<<"Invalid username or password!"<<endl;
                return false;
            }
        }//login
};//Admin

//class of the Product
class Product
{
    private:
        string _name;
        double _price;
        string _detail;
        int _storage;

    public:
        Product(string name,double price,int storage,string detail) : _name(name), _price(price), _detail(detail), _storage(storage) {}

        //get the name of the product
        string GetName() const {return _name;} 

        //get the price of the product
        double GetPrice() const {return _price;}

        //get the storage of the product
        int GetStorage() const {return _storage;}

        //get the detail of the product
        string GetDetail() const {return _detail;}

        //change the name of the product
        void SetName(const string &name) {this->_name=name;}

        //change the detail of the product
        void SetDetail(const string &detail) {this->_detail=detail;}

        //change the price of the product
        void SetPrice(double price) {this->_price=price;}

        //change the storage of the product
        void SetStorage(int storage) {this->_storage=storage;}
};//Product

class Cart
{
    private:
        vector<Product> cpdtdata;
        double money;
        string _account;
        string filename;

        //load the cart data from the file
        void loadCartFromFile()
        {
            ifstream file(filename,ios::in);
            if(file.is_open())
            {
                string line,word;
                int count=0;
            
                string name,detail,Price,Storage;
                double price;
                int storage;

                getline(file,line);

                istringstream sin;

                while(getline(file,line))
                {
                    sin.clear();
                    sin.str(line);
                    while(getline(sin,word,','))
                    {
                        if(count==0)
                        {
                            name=word;
                            count++;
                            continue;
                        }else if(count==1){
                            Price=word;
                            count++;
                            continue;
                        }else if(count==2){
                            Storage=word;
                            count++;
                            continue;
                        }else{
                            detail=word;
                            continue;
                        }
                    }
                    price=stod(Price);
                    storage=atoi(Storage.c_str());
                    cpdtdata.push_back(Product(name,price,storage,detail));
                }
                file.close();
            }else{
                cout<<"You haven't save a cart yet!"<<endl;
                system("pause");
            }
        }//loadCartFromFile

        //save the cart data to the file
        void saveCartToFile()
        {
            ofstream file(filename,ios::out);
            if(file.is_open())
            {
                file << "Product Name" << "," << "Price" << "," << "Storage" << "," << "Detail" << endl;

                for(auto &pdt:cpdtdata)
                {
                    file << pdt.GetName() << "," << pdt.GetPrice() << "," << pdt.GetStorage() << "," << pdt.GetDetail() << endl;
                }
                file.close();
            }
        }//saveCartToFile

    public:
        Cart(string account) : money(0) 
        {
            this->_account=account;
            this->filename=account+" Cart.csv";
            loadCartFromFile();
        }//Cart

        //add product into the cart
        void addProudct(Product &pdt,int num)
        {
            cpdtdata.push_back(Product(pdt.GetName(),pdt.GetPrice(),num,pdt.GetDetail()));
            pdt.SetStorage(pdt.GetStorage()-num);
        }//addProudct

        //delete product from cart
        int deleteProduct(string name)
        {
            int num;
            for(auto it = cpdtdata.begin(); it != cpdtdata.end(); ++it)
            {
                if((name.compare(it->GetName()))==0)
                {
                    num=it->GetStorage();
                    cpdtdata.erase(it);
                    saveCartToFile();
                    cout<<"The product has removed successfully!"<<endl;
                    return num;
                }
            }
            cout<<"There isn't such a product"<<endl;
            return 0;
        }//deleteProduct

        //modify the product storage
        int modifyStorage(string name,int num)
        {
            int diff;
            for(auto pdt:cpdtdata)
            {
                if((name.compare(pdt.GetName()))==0)
                {
                    diff=pdt.GetStorage()-num;
                    pdt.SetStorage(num);
                    saveCartToFile();
                    cout<<"The storage has modified successfully!"<<endl;
                    return diff;
                }
            }
            cout<<"There isn't such a product"<<endl;
            return 0;
        }//modifyStorage

        //list all the product in cart
        void ListCart()
        {
           cout<<"Products in Cart:"<<endl;
            cout << "|" << setw(20) << "<Product Name>" << "|" << setw(10) << "<Price>" << "|" << setw(10) << "<Storage>" << "|" << setw(60) << "<Detail>" << "|" << endl;
            for (const auto &pdt : cpdtdata)
            {
                cout << "|" << setw(20) << pdt.GetName() << "|" << setw(10) << pdt.GetPrice() << "|" << setw(10) << pdt.GetStorage() << "|" << setw(60) << pdt.GetDetail() << "|" << endl;
            } 
        }//ListCart
};//Cart

//class of the Customer
class User
{
    private:
        string _account;
        string _password;
        bool _isLoggedin;
        Cart buy_cart;
    
    public:
        User(string account,string password) : _account(account),_password(password),_isLoggedin(false),buy_cart(Cart(account)){}

        //get the Account
        string GetAccount() const {return _account;}

        //get the password
        string GetPassword() const {return _password;}

        //get the status of log
        bool getIsLoggedIn() const { return _isLoggedin; }

        //reset the password
        void setPassword(const string &password) { this->_password = password; }

        //set the status of log
        void setIsLoggedIn(bool isLoggedin) { this->_isLoggedin = isLoggedin; }

        void Add(Product &pdt,int num){this->buy_cart.addProudct(pdt,num);}

        int Del(string name)
        {
            int num;
            num=this->buy_cart.deleteProduct(name);
            return num;
        }//Del

        int Modify(string name,int num)
        {
            int diff;
            diff=this->buy_cart.modifyStorage(name,num);
            return diff;
        }//Modify

        void List(){this->buy_cart.ListCart();}
};//User

//the main shopping system
class SaleSystem
{
    private:
        Admin admin;
        vector<User> userdata;
        vector<Product> pdtdata;
        bool adminflag;

        Product searchpdt;          //for saving the product searched by user

        //save the search data
        void saveSearch(Product pdt)
        {
            searchpdt.SetDetail(pdt.GetDetail());
            searchpdt.SetName(pdt.GetName());
            searchpdt.SetPrice(pdt.GetPrice());
            searchpdt.SetStorage(pdt.GetStorage());
        }//saveSearch

        //load the customer data from the file
        void loadCustomerFromFile()
        {
            ifstream file("Customer");
            if(file.is_open())
            {
                string account,password;
                while(file>>account>>password)
                {
                    userdata.push_back(User(account,password));
                }
                file.close();
            }
        }//loadCustomerFromFile

        //save the customer data to the file
        void saveCustomerToFile()
        {
            ofstream file("Customer");
            if(file.is_open())
            {
                for(auto &user:userdata)
                {
                    file<<user.GetAccount()<<" "<<user.GetPassword()<<endl; 
                }
                file.close();
            }
        }//saveCustomerToFile

        //load the product data from the file
        void loadProductFromFile()
        {
            ifstream file("Product.csv",ios::in);
            if(file.is_open())
            {
                string line,word;
                int count=0;
            
                string name,detail,Price,Storage;
                double price;
                int storage;

                getline(file,line);

                istringstream sin;

                while(getline(file,line))
                {
                    sin.clear();
                    sin.str(line);
                    while(getline(sin,word,','))
                    {
                        if(count==0)
                        {
                            name=word;
                            count++;
                            continue;
                        }else if(count==1){
                            Price=word;
                            count++;
                            continue;
                        }else if(count==2){
                            Storage=word;
                            count++;
                            continue;
                        }else{
                            detail=word;
                            continue;
                        }
                    }
                    price=stod(Price);
                    storage=atoi(Storage.c_str());
                    pdtdata.push_back(Product(name,price,storage,detail));
                }
                file.close();
            }
        }//loadProductFromFile

        //save the product data to the file
        void saveProductToFile()
        {
            ofstream file("Product.csv",ios::out);
            if(file.is_open())
            {
                file << "Product Name" << "," << "Price" << "," << "Storage" << "," << "Detail" << endl;

                for(auto &pdt:pdtdata)
                {
                    file << pdt.GetName() << "," << pdt.GetPrice() << "," << pdt.GetStorage() << "," << pdt.GetDetail() << endl;
                }
                file.close();
            }
        }//saveProductToFile

    public:
        SaleSystem(string account,string password): admin(account,password),searchpdt("null",0,0,"null")
        {
            loadCustomerFromFile();
            loadProductFromFile();
            adminflag=false;
        }//SaleSystem

        string GetAdminName() const {return admin.GetAccount();}

        string GetUserName() const {
            for(auto &user:userdata)
            {
                if(user.getIsLoggedIn())
                    return user.GetAccount();
            }
            cerr<<"Some errors occurred!"<<endl;
            system("pause");
            exit(0);
        }//GetUserName

        //log in admin account
        bool AdminLogin(string account,string password)
        {
            if(admin.login(account,password))
            {
                adminflag=true;
                return true;
            }else{
                adminflag=false;
                return false;
            }
        }//AdminLogin

        //log out admin account
        bool AdminLogout()
        {
            if(adminflag==true)
            {
                adminflag=false;
                return true;
            }else{
                return false;
            }
        }//AdminLogout

        //log in customer account
        bool UserLogin(string account,string password)
        {
            for(auto &user:userdata)
            {
                if((account.compare((user.GetAccount()))==0) && (password.compare((user.GetPassword()))==0))
                {
                    user.setIsLoggedIn(true);
                    cout<<"User log in successfully"<<endl;
                    return true;
                }
            }
            cout<<"Invalid username or password!"<<endl;
            return false;
        }//UserLogin

        //log out user account
        void UserLogout()
        {
            for(auto &user:userdata)
                user.setIsLoggedIn(false); 
        }//UserLogout        

        //customer register
        void Register(string account,string password)
        {
            userdata.push_back(User(account,password));
            saveCustomerToFile();
            cout<<"Customer registers successfully"<<endl;
        }//Register

        //change the specified user's password
        void ChangePassword(string new_password)
        {
            for(auto &user:userdata)
            {
                if(user.getIsLoggedIn())
                {
                    user.setPassword(new_password);
                    saveCustomerToFile();
                    cout<<"The password has changed successfully"<<endl;
                    return ;
                }
            }
            cout<<"Invalid username or password!"<<endl;
            return ;
        }//ChangePassword

        //list all the products saved in the database
        void ListProducts(bool flag)
        {
            if(flag)
            {
                cout<<"Available Products:"<<endl;
                cout << "|" << setw(20) << "<Product Name>" << "|" << setw(10) << "<Price>" << "|" << setw(10) << "<Storage>" << "|" << setw(60) << "<Detail>" << "|" << endl;
                for (const auto &pdt : pdtdata)
                {
                    cout << "|" << setw(20) << pdt.GetName() << "|" << setw(10) << pdt.GetPrice() << "|" << setw(10) << pdt.GetStorage() << "|" << setw(60) << pdt.GetDetail() << "|" << endl;
                }
            }else{
                cout<<"Target Product:"<<endl;
                cout << "|" << setw(20) << "<Product Name>" << "|" << setw(10) << "<Price>" << "|" << setw(10) << "<Storage>" << "|" << setw(60) << "<Detail>" << "|" << endl;
                cout << "|" << setw(20) << searchpdt.GetName() << "|" << setw(10) << searchpdt.GetPrice() << "|" << setw(10) << searchpdt.GetStorage() << "|" << setw(60) << searchpdt.GetDetail() << "|" << endl;
            }
        }//ListProducts

        //add new product to the database
        void AddProduct(string name,double price,int storage,string detail)
        {
            pdtdata.push_back(Product(name,price,storage,detail));
            saveProductToFile();
            cout<<"Add new product successfully!"<<endl;
        }//AddProduct

        //delete the product
        void DeleteProduct(string name)
        {
            for(auto it = pdtdata.begin(); it != pdtdata.end(); ++it)
            {
                if((name.compare(it->GetName()))==0)
                {
                    pdtdata.erase(it);
                    saveProductToFile();
                    cout<<"The product has removed successfully!"<<endl;
                    return;
                }
            }
            cout<<"There isn't such a product"<<endl;
        }//DeleteProduct

        //modify the price of the product
        void ModifyPrice(string name,double price)
        {
            for(auto &pdt:pdtdata)
            {
                if((name.compare(pdt.GetName()))==0)
                {
                    pdt.SetPrice(price);
                    saveProductToFile();
                    cout<<"The price has modified successfully!"<<endl;
                    return;
                }
            }
        }//ModifyPrice

        //modify the storage of the product
        void ModifyStorage(string name,int storage)
        {
            for(auto &pdt:pdtdata)
            {
                if((name.compare(pdt.GetName()))==0)
                {
                    pdt.SetStorage(storage);
                    saveProductToFile();
                    cout<<"The storage has modified successfully!"<<endl;
                    return;
                }
            }
        }//ModifyStorage

        //modify the detail of the product
        void ModifyDetail(string name,string detail)
        {
            for(auto &pdt:pdtdata)
            {
                if((name.compare(pdt.GetName()))==0)
                {
                    pdt.SetDetail(detail);
                    saveProductToFile();
                    cout<<"The detail has modified successfully!"<<endl;
                    return;
                }
            }
        }//ModifyDetail

        //search and list all the products may wanted
        void SearchSystem(string keywords)
        {
            int jmpflag;
            clean();
            int min=5;          //the limit of the fuzzy search

            while(true)
            {
                clean();
                bool flag=false;
                //small interface
                cout<<"Here is the search mode given:"<<endl;
                cout<<"1. Accurate Search"<<endl;
                cout<<"2. Fuzzy Search"<<endl;
                cout<<"3. Cancel"<<endl;
                cout<<"Please input:";
                //switch the function
                cin>>jmpflag;

                clean();
                switch (jmpflag)
                {
                case 1:
                    for(auto pdt:pdtdata)
                    {
                        if((keywords.compare(pdt.GetName()))==0)
                        {
                            saveSearch(pdt);
                            ListProducts(false);
                            return;
                        }
                    }
                    cout<<"Using Accurate Search can't find the product!"<<endl;
                    return;
                case 2:
                    for(auto pdt:pdtdata)
                    {
                        if(levenshtein_distance(trim(pdt.GetName()),trim(keywords))<min)
                        {
                            flag=true;
                            saveSearch(pdt);
                            min=levenshtein_distance(trim(pdt.GetName()),trim(keywords));
                        }
                    }
                    if(flag)
                    {
                        ListProducts(false);
                    }else{
                        cout<<"Using Fuzzy Search can't find the product!"<<endl;
                    }
                    return;
                case 3:
                    cout<<"returning..."<<endl;
                    return;
                default:
                    cout<<"Invalid input, please try again!"<<endl;
                    system("pause");
                }
            }
        }//SearchSystem

        bool TestExsist(string name)
        {
            for(auto &pdt:pdtdata)
            {
                if((name.compare(pdt.GetName()))==0)
                {
                    return true;
                }
            }
            return false;
        }//TestExsist

};//SaleSystem

SaleSystem ModifyInterface(SaleSystem mainsystem,string name)
{
    clean();
    int jmpflag;
    double price;
    int storage;
    string detail;

    if(!(mainsystem.TestExsist(name)))
    {
        cout<<"There isn't such a product!"<<endl;
        system("pause");
        return mainsystem;
    }

    while(true)
    {
        clean();
        //the modify interface
        cout<<"Here are the data you could modify"<<endl;
        cout<<"1. Price"<<endl;
        cout<<"2. Storage"<<endl;
        cout<<"3. Detail"<<endl;
        cout<<"4. Cancel/Done"<<endl;
        cout<<"Please input:";
        //switch the function
        cin>>jmpflag;

        clean();
        switch(jmpflag)
        {
            case 1:
                cout<<"Please input the price you want to modify:";
                cin>>price;
                mainsystem.ModifyPrice(name,price);
                break;
            case 2:
                cout<<"Please input the storage you want to modify:";
                cin>>storage;
                mainsystem.ModifyStorage(name,storage);
                break;
            case 3:
                cout<<"Please input the detail you want to modify:"<<endl;
                getline(cin,detail);
                mainsystem.ModifyDetail(name,detail);
                break;
            case 4:
                cout<<"returning..."<<endl;
                return mainsystem;
            default:
                cout<<"Invalid input, please try again!"<<endl;
                system("pause");
                continue;
        }
        system("pause");
    }
}//ModifyInterface

SaleSystem AdminInterface(SaleSystem mainsystem)
{
    clean();
    int jmpflag;
    string name;
    double price;
    int storage;
    string detail;

    while(true)
    {
        clean();
        //the admin interface
        cout<<"Admin System ver "<<VERSION<<endl;
        cout<<"Welcome back, Administrator <"<<mainsystem.GetAdminName()<<">"<<endl<<endl;
        cout<<"1. Add Product"<<endl;
        cout<<"2. Remove Product"<<endl;
        cout<<"3. Modify Product"<<endl;
        cout<<"4. Log out"<<endl;
        cout<<"Please input:";
        //switch the function
        cin>>jmpflag;

        clean();
        switch(jmpflag)
        {
            case 1:
                cout<<"Please input the information about the new product"<<endl;
                cout<<"Product Name:";
                getline(cin,name);
                cout<<"Product Price:";
                cin>>price;
                cout<<"Product Storage:";
                cin>>storage;
                fflush(stdin);
                cout<<"Product Detail:";
                getline(cin,detail);

                mainsystem.AddProduct(name,price,storage,detail);
                clean();
                cout<<"The new product has been added"<<endl;
                break;
            case 2:
                cout<<"Please input the name of the product you want to remove:"<<endl;
                getline(cin,name);

                mainsystem.DeleteProduct(name);
                clean();
                cout<<"The product has been removed!"<<endl;
                break;
            case 3:
                cout<<"Please input the name of the product you want to modify:"<<endl;
                getline(cin,name);

                mainsystem=ModifyInterface(mainsystem,name);
                break;
            case 4:
                mainsystem.AdminLogout();
                cout<<"return to the main system..."<<endl;
                return mainsystem;
            default:
                cout<<"Invalid input, please try again!"<<endl;
                break;
        }
        system("pause");
    }
}//AdminInterface

SaleSystem UserInterface(SaleSystem mainsystem)
{
    clean();
    string new_password;
    int jmpflag;
    string keywords;

    while(true)
    {
        clean();
        //the user interface
        cout<<"User System ver "<<VERSION<<endl;
        cout<<"Hello, User <"<<mainsystem.GetUserName()<<">"<<endl<<endl;
        cout<<"1. Change the password"<<endl;
        cout<<"2. Search Product"<<endl;
        cout<<"3. Log out"<<endl;
        cout<<"Please input:";
        //switch the function
        cin>>jmpflag;

        clean();
        switch(jmpflag)
        {
            case 1:
                cout<<"Please input the new password you want:";
                cin>>new_password;
                mainsystem.ChangePassword(new_password);
                break;     
            case 2:
                cout<<"Please input the keywords of the product:";
                getline(cin,keywords);
                mainsystem.SearchSystem(keywords);
                break;
             case 3:
                mainsystem.UserLogout();
                cout<<"return to the main system..."<<endl;
                return mainsystem;
            default:
                cout<<"Invalid input, please try again!"<<endl;
                break;
        }
        system("pause");
    }
}//UserInterface

//main interface of user/admin
int main()
{
    SaleSystem mainsystem("CrazyApple","332525Gao");
    string account,password;
    int jmpflag;
    bool run=true;

    //welcome infomation
    cout<<"Welcome to use the shopping system!"<<endl;
    cout<<"The version of this system is "<<VERSION<<endl;
    system("pause");
    clean();

    while(run)
    {
        clean();
        //the main interface
        cout<<"You could input the number of the label to use this system:"<<endl;
        cout<<"1. Admin log in"<<endl;
        cout<<"2. User log in"<<endl;
        cout<<"3. User register"<<endl;
        cout<<"4. Show the products list"<<endl;
        cout<<"5. Exit the system"<<endl;
        cout<<"Please input:";
        //switch the function or interface
        cin>>jmpflag;

        clean();
        switch(jmpflag)
        {
            case 1:
                //get the account and the password
                cout<<"Please input the information of the admin"<<endl;
                cout<<"account: ";
                cin>>account;
                cout<<"password: ";
                cin>>password;

                if(mainsystem.AdminLogin(account,password))
                {
                    system("pause");
                    mainsystem=AdminInterface(mainsystem);             //get into the admin interface
                }
                break;

            case 2:
                //get the account and the password
                cout<<"Please input the information of the registered user"<<endl;
                cout<<"account: ";
                cin>>account;
                cout<<"password: ";
                cin>>password;

                if(mainsystem.UserLogin(account,password))
                {
                    system("pause");
                    mainsystem=UserInterface(mainsystem);              //get into the user interface
                }
                break;

            case 3:
                //get the register data
                cout<<"Please input the information of a new user"<<endl;
                cout<<"account: ";
                cin>>account;
                cout<<"password: ";
                cin>>password;

                clean();

                mainsystem.Register(account,password);
                break;

            case 4:
                //list all the product for view
                mainsystem.ListProducts(true);
                break;

            case 5:
                //exit the system
                run=false;
                cout<<"Thanks for using this system, see you next time!"<<endl;
                break;

            default:
                //error handle
                cout<<"Invalid input, please try again!"<<endl;
                break;
        }//switch
        system("pause");
    }

    return 0;                       //exit the system
}//main
