import os
import tempfile
import urllib.request
import http.cookiejar
import argparse
from pathlib import Path

def get_masses_input_from_file(path: str):
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

        total_fuel = sum(mass // 3 - 2 for mass in masses)

        print(f"\nTotal fuel required: {total_fuel}")

    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()