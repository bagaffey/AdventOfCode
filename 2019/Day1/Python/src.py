import math
import os

def calculate_fuel_requirement(mass):
    """
    Calculates the required fuel for a module based upon its mass.
    """
    return (math.floor(mass / 3) - 2)

def main():
    total_fuel = 0
    
    print(os.getcwd())

    try:
        with open("./2019/Day1/input_20191201.txt", "r") as file:
            for line in file:
                mass = int(line.strip())
                fuel_required = calculate_fuel_requirement(mass)
                total_fuel += fuel_required
                print(f"Mass: {mass}, Fuel needed: {fuel_required}")

        print(f"\nTotal fuel required: {total_fuel}")

    except FileNotFoundError:
        print("Error: input file was not found.")
    except ValueError as e:
        print(f"Error encountered reading file: {e}")

if __name__ == "__main__":
    main()