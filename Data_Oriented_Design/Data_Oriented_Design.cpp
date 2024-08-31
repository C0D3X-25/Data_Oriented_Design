/*
	In a computer there is memories space who are faster to access than other:
		- CPU Registers		=> Level-0, the fastest
		- Cache				=> Level-1, what DOD (Data Oriented Design) try to manage the best
		- Main Memory		=> Level-2, what OOP using the most
		- Secondary	Storage	=> Level-3, 
		- Tertiary Storage	=> Level-4, 
	
	DOD dont use STL because it's OOP, maximise arrays instead
*/



#include <iostream>
#include <chrono>

struct Vec3f {
	float m_x, m_y, m_z;
};


// ============ OOP ============
class Car {
public:
	float m_x, m_y;
	float m_velocity_x;
	float m_velocity_y;
	float m_max_speed;
	Vec3f m_rgb;

	Car(float x, float y, float max_speed) 
		: m_x{x}, m_y{y}, m_max_speed{max_speed}
	{
		m_velocity_x = 0;
		m_velocity_y = 0;
		m_rgb = { 255, 255, 255 };
	}

	~Car() {};

	// Here for example when trying to update the position, 
	//  the cache line is pollute by the others members of car who are not used.
	void updatePosition() {
		m_x += m_velocity_x;
		m_y += m_velocity_y;
	}

	void updateVelocity(int vel_x, int vel_y) {
		if (m_velocity_x + vel_x < m_max_speed) {
			m_velocity_x += m_max_speed;
		}
		if (m_velocity_y + vel_y < m_max_speed) {
			m_velocity_y += m_max_speed;
		}
	}

	void set_Rgb(Vec3f rgb) {
		m_rgb = rgb;
	}
};

Car cars[5] = {
	Car(0,0,100),
	Car(5,-1,100),
	Car(-100,32,200),
	Car(567,4,120),
	Car(-100,-230,150),
};

void loop() {
	for (size_t i = 0; i < 5; i++) {
		cars[i].updateVelocity(25, 0);
		cars[i].updatePosition();
	}
}



// ============ DOD ============

// The Struct/Class is not needed, instead DOD use array

//	== Here the data arrays replacing Class ==
//	float _x[5];
//	float _y[5];
//	float velocity_x[5];
//	float velocity_y[5];
//	float max_speed[5];
//	Vec3f rgb[5];

void updatePos(float* vel_x, float* vel_y, float* pos_x, float* pos_y, int count) {
	for (size_t i = 0; i < count; i++) {
		pos_x[i] += vel_x[i];
		pos_y[i] += vel_y[i];
	}
}

void updateVel(float* vel_x, float* vel_y, float* max_speed, float increase_x, float increase_y, int count) {
	for (size_t i = 0; i < count; i++) {
		if (vel_x[i] + increase_x < max_speed[i]) {
			vel_x[i] += increase_x;
		}
		if (vel_y[i] + increase_y < max_speed[i]) {
			vel_y[i] += increase_y;
		}
	}
}

// Same as an array of Car Objects, but data are spread in differant arrays
float _x[5] = { 0, 5, -100, 567, -100, };
float _y[5] = { 0, -1, 32, 4, -230, };
float max_speed[5] = { 100, 100, 200, 120, 150 };
float velocity_x[5] = { 0, 0, 0, 0, 0 };
float velocity_y[5] = { 0, 0, 0, 0, 0 };
Vec3f rgb[5] = { {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255} };




// ============ Main ============
long long timeOOP(const int nbr_test) {

	long long total{ 0 };

	for (size_t j = 0; j < nbr_test; j++) {

		auto start = std::chrono::high_resolution_clock::now();

		// Start
		for (size_t i = 0; i < 1000; i++) {
			loop();
		}
		// End

		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

		total += duration.count();

		std::cout << "Time taken by OOP function: " << duration.count() << " microsecond\n";
	}
	return total;
}

long long timeDOD(const int nbr_test) {

	long long total{ 0 };

	for (size_t j = 0; j < nbr_test; j++) {

		auto start = std::chrono::high_resolution_clock::now();

		// Start
		for (size_t i = 0; i < 1000; i++) {
			updateVel(velocity_x, velocity_y, max_speed, 25, 0, 5);
			updatePos(velocity_x, velocity_y, _x, _y, 5);
		}
		// End

		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

		total += duration.count();

		std::cout << "Time taken by DOD function: " << duration.count() << " microsecond\n";
	}
	return total;
}

int main() {

	const int TEST_COUNT{ 500 }; // * 1000

	long long total_oop = timeOOP(TEST_COUNT);
	long long total_dod = timeDOD(TEST_COUNT);

	std::cout << "\n\nFor OOP :Total: " << total_oop << " microsecond | Average: " << (total_oop / TEST_COUNT) << " microsecond\n";
	std::cout << "For DOD :Total: " << total_dod << " microsecond | Average: " << (total_dod / TEST_COUNT) << " microsecond\n";

	std::cout << "\n\nFor " << (TEST_COUNT * 1000) << " test run, DOD is " << (total_oop - total_dod) 
		<< " microsecond(" << (total_oop - total_dod) / pow(10, 3) << " millisec) faster than OOP\n"
		<< "and " << (total_oop / TEST_COUNT) - (total_dod / TEST_COUNT) << " microsecond faster in Average\n";

	return 0;
}
