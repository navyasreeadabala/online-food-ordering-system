#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cctype>
using namespace std;

class MenuItem {
private:
    int id;
    string name;
protected:
    double basePrice;
public:
    MenuItem(int id, string name, double basePrice) {
        this->id = id;
        this->name = name;
        this->basePrice = basePrice;
    }
    int getId() const { return id; }
    string getName() const { return name; }
    virtual double getPrice() const = 0;
    virtual string getCategory() const = 0;
    virtual ~MenuItem() {}
};

class Pizza : public MenuItem {
public:
    Pizza(int id, string name, double price) : MenuItem(id, name, price) {}
    double getPrice() const override { return basePrice + 20; }
    string getCategory() const override { return "Pizza"; }
};

class Burger : public MenuItem {
public:
    Burger(int id, string name, double price) : MenuItem(id, name, price) {}
    double getPrice() const override { return basePrice + 10; }
    string getCategory() const override { return "Burger"; }
};

class Drink : public MenuItem {
public:
    Drink(int id, string name, double price) : MenuItem(id, name, price) {}
    double getPrice() const override { return basePrice; }
    string getCategory() const override { return "Drink"; }
};

class Dessert : public MenuItem {
public:
    Dessert(int id, string name, double price) : MenuItem(id, name, price) {}
    double getPrice() const override { return basePrice + 5; }
    string getCategory() const override { return "Dessert"; }
};

class Customer {
private:
    string name;
    string address;
public:
    Customer(string name, string address) {
        this->name = name;
        this->address = address;
    }
    string getName() const { return name; }
    string getAddress() const { return address; }
};

class Discount {
public:
    virtual double getDiscount(double amount) const = 0;
    virtual string getName() const = 0;
    virtual ~Discount() {}
};

class NoDiscount : public Discount {
public:
    double getDiscount(double amount) const override { return 0; }
    string getName() const override { return "None"; }
};

class PercentDiscount : public Discount {
private:
    int percent;
public:
    PercentDiscount(int percent) { this->percent = percent; }
    double getDiscount(double amount) const override { return amount * percent / 100; }
    string getName() const override { return to_string(percent) + "% off"; }
};

class FlatDiscount : public Discount {
private:
    double off;
public:
    FlatDiscount(double off) { this->off = off; }
    double getDiscount(double amount) const override { return off < amount ? off : amount; }
    string getName() const override { return "Rs. " + to_string((int)off) + " off"; }
};

class Payment {
public:
    virtual double calculateTotal(double amount) const = 0;
    virtual string getMethod() const = 0;
    virtual ~Payment() {}
};

class CashPayment : public Payment {
public:
    double calculateTotal(double amount) const override { return amount; }
    string getMethod() const override { return "Cash on Delivery"; }
};

class CardPayment : public Payment {
public:
    double calculateTotal(double amount) const override { return amount + amount * 0.02; }
    string getMethod() const override { return "Card (2% fee)"; }
};

class UpiPayment : public Payment {
public:
    double calculateTotal(double amount) const override { return amount - amount * 0.05; }
    string getMethod() const override { return "UPI (5% discount)"; }
};

class Order {
private:
    vector<MenuItem*> items;
    vector<int> quantities;
    Discount* coupon;
public:
    Order() { coupon = new NoDiscount(); }

    ~Order() { delete coupon; }

    void addItem(MenuItem* item, int qty) {
        for (size_t i = 0; i < items.size(); i++) {
            if (items[i]->getId() == item->getId()) {
                quantities[i] += qty;
                return;
            }
        }
        items.push_back(item);
        quantities.push_back(qty);
    }

    bool removeItem(int id) {
        for (size_t i = 0; i < items.size(); i++) {
            if (items[i]->getId() == id) {
                items.erase(items.begin() + i);
                quantities.erase(quantities.begin() + i);
                return true;
            }
        }
        return false;
    }

    void setCoupon(Discount* newCoupon) {
        delete coupon;
        coupon = newCoupon;
    }

    string getCouponName() const { return coupon->getName(); }

    bool isEmpty() const { return items.empty(); }

    double getSubtotal() const {
        double total = 0;
        for (size_t i = 0; i < items.size(); i++)
            total += items[i]->getPrice() * quantities[i];
        return total;
    }

    double getDiscountAmount() const { return coupon->getDiscount(getSubtotal()); }

    double getGst() const { return (getSubtotal() - getDiscountAmount()) * 0.05; }

    double getDeliveryCharge() const { return getSubtotal() >= 500 ? 0 : 40; }

    double getTotal() const {
        return getSubtotal() - getDiscountAmount() + getGst() + getDeliveryCharge();
    }

    void showCart(ostream& out) const {
        if (items.empty()) {
            out << "\nYour cart is empty.\n";
            return;
        }
        out << "\n---------------- YOUR CART ----------------\n";
        out << left << setw(20) << "Item" << setw(6) << "Qty" << setw(10) << "Price" << "Total\n";
        for (size_t i = 0; i < items.size(); i++) {
            out << left << setw(20) << items[i]->getName()
                << setw(6) << quantities[i]
                << setw(10) << items[i]->getPrice()
                << items[i]->getPrice() * quantities[i] << "\n";
        }
        out << "Subtotal: Rs. " << getSubtotal() << "\n";
    }
};

class Restaurant {
private:
    vector<MenuItem*> menu;
public:
    Restaurant() {
        menu.push_back(new Pizza(1, "Margherita Pizza", 200));
        menu.push_back(new Pizza(2, "Veg Supreme Pizza", 280));
        menu.push_back(new Burger(3, "Veg Burger", 90));
        menu.push_back(new Burger(4, "Cheese Burger", 120));
        menu.push_back(new Drink(5, "Cold Coffee", 80));
        menu.push_back(new Drink(6, "Lemon Soda", 50));
        menu.push_back(new Dessert(7, "Choco Brownie", 90));
        menu.push_back(new Dessert(8, "Ice Cream", 60));
    }

    ~Restaurant() {
        for (size_t i = 0; i < menu.size(); i++)
            delete menu[i];
    }

    void showMenu() const {
        cout << "\n------------------- MENU -------------------\n";
        cout << left << setw(5) << "ID" << setw(22) << "Name" << setw(10) << "Type" << "Price\n";
        for (size_t i = 0; i < menu.size(); i++) {
            cout << left << setw(5) << menu[i]->getId()
                 << setw(22) << menu[i]->getName()
                 << setw(10) << menu[i]->getCategory()
                 << menu[i]->getPrice() << "\n";
        }
    }

    MenuItem* findItem(int id) const {
        for (size_t i = 0; i < menu.size(); i++)
            if (menu[i]->getId() == id)
                return menu[i];
        return nullptr;
    }
};

int readInt() {
    int value;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please enter a valid number: ";
    }
    return value;
}

void applyCoupon(Order& order) {
    if (order.isEmpty()) {
        cout << "\nAdd items to your cart before applying a coupon.\n";
        return;
    }

    string code;
    cout << "\nAvailable coupons: FOOD10 (10% off), SAVE20 (20% off), FLAT50 (Rs. 50 off)\n";
    cout << "Enter coupon code: ";
    cin >> code;
    for (size_t i = 0; i < code.size(); i++)
        code[i] = toupper(code[i]);

    if (code == "FOOD10") order.setCoupon(new PercentDiscount(10));
    else if (code == "SAVE20") order.setCoupon(new PercentDiscount(20));
    else if (code == "FLAT50") order.setCoupon(new FlatDiscount(50));
    else {
        cout << "Invalid coupon code.\n";
        return;
    }
    cout << "Coupon applied: " << order.getCouponName() << "\n";
}

void printBill(ostream& out, const Customer& customer, const Order& order, const Payment& payment) {
    out << fixed << setprecision(2);
    double total = order.getTotal();

    out << "\n=============== FINAL BILL ===============";
    order.showCart(out);
    out << "Coupon          : " << order.getCouponName() << "\n";
    out << "Discount        : - Rs. " << order.getDiscountAmount() << "\n";
    out << "GST (5%)        : + Rs. " << order.getGst() << "\n";
    out << "Delivery Charge : + Rs. " << order.getDeliveryCharge() << "\n";
    out << "Payment Method  : " << payment.getMethod() << "\n";
    out << "Payment Change  : Rs. " << payment.calculateTotal(total) - total << "\n";
    out << "------------------------------------------\n";
    out << "Customer        : " << customer.getName() << "\n";
    out << "Address         : " << customer.getAddress() << "\n";
    out << "TOTAL TO PAY    : Rs. " << payment.calculateTotal(total) << "\n";
    out << "==========================================\n";
}

bool checkout(const Customer& customer, const Order& order) {
    if (order.isEmpty()) {
        cout << "\nYour cart is empty. Add some items before checkout.\n";
        return false;
    }

    Payment* payment = nullptr;
    while (payment == nullptr) {
        cout << "\nChoose payment method:\n1. Cash\n2. Card\n3. UPI\n0. Back to main menu\nEnter choice: ";
        int choice = readInt();

        if (choice == 1) payment = new CashPayment();
        else if (choice == 2) payment = new CardPayment();
        else if (choice == 3) payment = new UpiPayment();
        else if (choice == 0) return false;
        else cout << "Invalid payment choice. Try again.\n";
    }

    printBill(cout, customer, order, *payment);

    ofstream file("bill.txt");
    printBill(file, customer, order, *payment);
    file.close();

    cout << "\nOrder placed successfully. Your bill is saved in bill.txt\n";

    delete payment;
    return true;
}

int main() {
    cout << fixed << setprecision(2);

    string name, address;
    cout << "===== ONLINE FOOD ORDERING SYSTEM =====\n";
    cout << "Enter your name: ";
    getline(cin, name);
    cout << "Enter your address: ";
    getline(cin, address);

    Customer customer(name, address);
    Restaurant restaurant;
    Order order;

    int choice;
    do {
        cout << "\n1. View Menu";
        cout << "\n2. Add Item to Cart";
        cout << "\n3. Remove Item from Cart";
        cout << "\n4. View Cart";
        cout << "\n5. Apply Coupon";
        cout << "\n6. Checkout";
        cout << "\n0. Exit";
        cout << "\nEnter choice: ";
        choice = readInt();

        if (choice == 1) {
            restaurant.showMenu();
        } else if (choice == 2) {
            int id, qty;
            cout << "Enter item ID: ";
            id = readInt();
            cout << "Enter quantity: ";
            qty = readInt();
            MenuItem* item = restaurant.findItem(id);
            if (item != nullptr && qty > 0) {
                order.addItem(item, qty);
                cout << item->getName() << " added to cart.\n";
            } else {
                cout << "Invalid item or quantity.\n";
            }
        } else if (choice == 3) {
            cout << "Enter item ID to remove: ";
            int id = readInt();
            if (order.removeItem(id))
                cout << "Item removed from cart.\n";
            else
                cout << "That item is not in your cart.\n";
        } else if (choice == 4) {
            order.showCart(cout);
        } else if (choice == 5) {
            applyCoupon(order);
        } else if (choice == 6) {
            if (checkout(customer, order))
                break;
        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    cout << "\nGoodbye, " << customer.getName() << "!\n";
    return 0;
}