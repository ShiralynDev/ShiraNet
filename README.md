# ShiraNet
ShiraNet is a versatile, flexible and modern C++ networking library with varying level of control made as an alternative to other networking libraries like Boost.Asio. ShiraNet is made using C++ version 20 and uses a simple syntax to allow for a quick understanding and a shallow learning curve.

## Support
ShiraNet supports Linux/Unix aswell as Windows.

## Varying level of control
ShiraNet is built with a class hierarchy that allows for the developer to select the amount of control they want over certain parts. As an example a developer can choose between using a premade TcpSocket class that inherits the Socket class but they can also create their own TcpSocket implementation based on the raw Socket class. The library for example has a TcpServer class which allows for a set of already implemented functions to be used but a developer can also choose to make their own Server based directly on the TcpSocket class.