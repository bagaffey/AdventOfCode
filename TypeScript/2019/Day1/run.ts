/*
deno run `
   --allow-env=AOC_SESSION,TEMP,TMP `
   --allow-read `
   --allow-write `
   --allow-net=adventofcode.com `
   .\TypeScript\2019\Day1\run.ts

*/

import { join } from "https://deno.land/std/path/mod.ts";

async function getMassesFromFile(Path: string): Promise<number[]> {
    try {
        const Text = await Deno.readTextFile(Path);
        return Text
        .replace(/\r\n/g, "\n")
        .split("\n")
        .map( (s) => s.trim() )
        .filter( (s) => s.length > 0 && /^\d+$/.test(s) )
        .map( (s) => Number(s) );
    } catch {
        throw new Error (`Input file not found: ${Path}`);
    }
}

async function getMassesFromAOC(CacheFile: string): Promise<number[]> {
    const Session = Deno.env.get("AOC_SESSION");
    if (!Session) 
        throw new Error("AOC_SESSION environment variable is not set. Set it and run again.")

    const URL = "https://adventofcode.com/2019/day/1/input";
    const Resp = await fetch(URL, {
        headers: {
            "Cookie": `session=${Session}`,
            "User-Agent": "aoc-fetcher/1.0",
        },
    });

    if (!Resp.ok)
        throw new Error(`Failed to fetch AoC input: ${Resp.status} ${Resp.statusText}`);

    const Downloaded = (await Resp.text()).replace(/\r\n/g, "\n");
    const Normalized = Downloaded.trimEnd() + "\n";
    await Deno.mkdir(dirname(CacheFile), { recursive: true });
    await Deno.writeTextFile(CacheFile, Normalized, { create: true });

    return Normalized
    .split("\n")
    .map( (s) => s.trim() )
    .filter( (s) => s.length > 0 && /^\d+$/.test(s) )
    .map( (s) => Number(s) );
}

function dirname(p: string): string {
    const idx = p.replace(/\\/g, "/").lastIndexOf("/");
    return (idx === -1 ? "." : p.slice(0, idx));
}

async function getMassesFromCacheOrRedownload(): Promise<number[]> {
    const Temp =
    Deno.env.get("TEMP") ??
    Deno.env.get("TMP") ??
    (() => {
        throw new Error("TEMP and TMP not found in environment.");
    })();

    const CacheDir = join(Temp, "advent-of-code");
    const CacheFile = join(CacheDir, "2019-01.dat");

    try {
        const FileStats = await Deno.stat(CacheFile);
        
        if (FileStats.isFile) {
            const Text = await Deno.readTextFile(CacheFile);
            return Text
            .replace(/\r\n/g, "\n")
            .split("\n")
            .map( (s) => s.trim() )
            .filter( (s) => s.length > 0)
            .map( (s) => Number(s) );
        }
    } catch {
        // File was not found.
    }

    return await getMassesFromAOC(CacheFile);
}

function calculateFuelRequirementForModuleMass(m: number): number {
  return Math.floor(m / 3) - 2;
}

function calculateRealFuelRequirement(m: number): number {
  const fuel = Math.floor(m / 3) - 2;
  if (fuel <= 0) return 0;
  return fuel + calculateRealFuelRequirement(fuel);
}

if (import.meta.main) {
    try {
        const InputPath = Deno.args[0]; // optional argument, file path to a data file
        const Masses = InputPath ? await getMassesFromFile(InputPath) : await getMassesFromCacheOrRedownload();

        const TotalModulesFuel = Masses.map(calculateFuelRequirementForModuleMass)
        .reduce( (a,b) => a + b, 0);

        console.log(`\n(Part 1) - Transport masses fuel requirement: ${TotalModulesFuel}`);

        const TotalTripFuelRequirement = Masses.map(calculateRealFuelRequirement)
        .reduce( (a,b) => a + b, 0);

        console.log(`\n(Part 2) - Total Fuel for trip required: ${TotalTripFuelRequirement}\n`);
    } catch (e) {
        console.error(`Error: ${e instanceof Error ? e.message : String(e)}`);
        Deno.exit(1);
    }
}