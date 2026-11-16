# online-food-ordering-system
Console-based food ordering system in C++ built to demonstrate OOP: abstraction, inheritance, and polymorphism. Features menu, cart, coupons, GST, delivery charges, Cash/Card/UPI payments, and bill export to a text file.
# Online Food Ordering System (C++)

A console-based food ordering system built in C++ to demonstrate
Object-Oriented Programming concepts.

## Features
- View menu with Pizza, Burger, Drink, and Dessert categories
- Add and remove items, view cart
- Coupon codes: FOOD10 (10% off), SAVE20 (20% off), FLAT50 (Rs. 50 off)
- 5% GST, with free delivery on orders of Rs. 500 or more (Rs. 40 otherwise)
- Payment options: Cash, Card (2% fee), UPI (5% discount)
- Final bill printed on screen and saved to bill.txt
- Input validation for invalid choices and numbers

## OOP Concepts Used
- **Abstraction:** abstract classes MenuItem, Discount, and Payment
- **Inheritance:** Pizza, Burger, Drink, and Dessert extend MenuItem
- **Polymorphism:** virtual functions for pricing, discounts, and payments
- **Encapsulation:** private data members with getters
- **File Handling:** bill export using ofstream

## How to Run
g++ food.cpp -o food
./food

## Future Improvements
- Load the menu from a file or database
- Order history and user accounts
- GUI or web interface
