#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>

std::atomic<int> clients = 0;
int clients_toDo = 0;
bool full_capacity = false;

void client(int duration) {
	int client_numbers;
	while (!full_capacity) {
		client_numbers = clients.load();
		if (client_numbers < clients_toDo) {
			full_capacity = false;
			client_numbers++;
			clients.store(client_numbers);
			std::cout << "Пришел клиент " << client_numbers << "\n";
		}
		else {
			full_capacity = true;
		}
		std::this_thread::sleep_for(std::chrono::seconds(duration));
	}
}

void operationist(int duration) {
	int client_numbers;
	client_numbers = clients.load();
	while (client_numbers >= 0) {
		client_numbers--;
		clients.store(client_numbers);
		std::this_thread::sleep_for(std::chrono::seconds(duration));
	}
}

int main() {
	int inputs;

	std::cout << "Введите число клиентов: ";
	std::cin >> inputs;

	clients_toDo = inputs;


	std::thread th1(client, 1);
	std::thread th2(operationist, 2);


	th1.join();
	std::cout << "Пришло максимальное колиество клиентов\n";

	th2.join();
	std::cout << "Обслуживание завершено\n";

	return 0;
}