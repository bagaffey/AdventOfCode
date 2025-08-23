import os
import tempfile
import urllib.request
import http.cookiejar
import argparse
from pathlib import Path

def get_masses_input_from_file(path: str) -> list[int]:
    p = Path(path)
    if not p.exists():
        raise FileNotFoundError(f"Input file not found: {path}")
    with p.open("r", encoding="utf-8") as input_file:
        return [int(line.strip()) for line in input_file if line.strip().isdigit()]
    
def get_masses_input_from_aoc(cached_data_file: Path) -> list[int]:
    session_cookie = os.getenv("AOC_SESSION")
    if not session_cookie:
        raise RuntimeError("AOC_SESSION environment variable is not set. Set and run again.")
    
    url = "https://adventofcode.com/2019/day/1/input"
    mycookiejar = http.cookiejar.CookieJar()
    opener = urllib.request.build_opener(urllib.request.HTTPCookieProcessor(mycookiejar))
    opener.addheaders = [("Cookie", f"session={session_cookie}"),
                         ("User-Agent", "aoc-fetcher/1.0")]

    with opener.open(url) as response:
        downloaded_input = response.read().decode("utf-8")

    cached_data_file.write_text(downloaded_input.rstrip("\n") + "\n", encoding="utf-8")

    return [int(line) for line in downloaded_input.splitlines() if line.strip().isdigit()]

def get_masses_input_from_cache_or_redownload() -> list[int]:
    cached_data = Path(tempfile.gettempdir() + "\\2019-12-01-1.dat")
    if cached_data.exists():
        with cached_data.open("r", encoding="utf-8") as input_file:
            return [int(line.strip()) for line in input_file if line.strip()]
    else:
        return get_masses_input_from_aoc(cached_data)
    
def calculate_fuel_requirement_for_module_mass(module_mass: int) -> int:
    return (module_mass // 3 - 2)
    
def calculate_real_fuel_requirement(module_mass: int) -> int:
    fuel_fuel = module_mass // 3 - 2
    if fuel_fuel <= 0:
        return (0)
    else:
        fuel_fuel += calculate_real_fuel_requirement(fuel_fuel)
        return (fuel_fuel)

def main():
    parser = argparse.ArgumentParser(description="Advent of Code 2019 Day 1 Fuel Calculator")
    parser.add_argument(
        "input_path",
        nargs="?",
        default=None,
        help="Optional path to input file. If not provided, input will be fetched from Advent of Code site using AOC_SESSION env var as session cookie."
    )
    args = parser.parse_args()

    try:
        if args.input_path:
            masses = get_masses_input_from_file(args.input_path)
        else:
            masses = get_masses_input_from_cache_or_redownload()

        total_modules_fuel = sum(calculate_fuel_requirement_for_module_mass(mass) for mass in masses)

        print(f"\n(Part 1) - Transport masses fuel required: {total_modules_fuel}")

        total_trip_fuel_requirement = sum(calculate_real_fuel_requirement(mass) for mass in masses)
        
        print(f"\n(Part 2) - Total Fuel for trip required: {total_trip_fuel_requirement}")

    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()