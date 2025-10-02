// deno run --allow-env=AOC_SESSION,TEMP,TMP --allow-read --allow-write --allow-net=adventofcode.com Day2.ts [inputFile]

import { join } from "https://deno.land/std/path/mod.ts";

enum DimensionIndex {
  Length = 0,
  Width = 1,
  Height = 2,
}

type present_dimensions = {
  length: number;
  width: number;
  height: number;
};

function getSortedPresentDimensions(p: present_dimensions): number[] {
  return [p.length, p.width, p.height].sort( (a, b) => a - b );
}

async function getPuzzleFromFile(path: string): Promise<string> {
  try {
    const text = await Deno.readTextFile(path);
    return normalizeText(text);
  } catch {
    throw new Error(`Input file not found: ${path}`);
  }
}

async function getPuzzleFromAOC(cacheFile: string, url: string): Promise<string> {
  const session = Deno.env.get("AOC_SESSION");
  if (!session) throw new Error("AOC_SESSION environment variable is not set. Set it and run again.");

  const Response = await fetch(url, {
    headers: {
      "Cookie": `session=${session}`,
      "User-Agent": "aoc-fetcher/1.0",
    },
  });

  if (!Response.ok) {
    throw new Error(`Failed to fetch AoC input: ${Response.status} ${Response.statusText}`);
  }

  const RawData = (await Response.text()).replace(/\r\n/g, "\n");
  const NormalizedInput = normalizeText(RawData);

  await Deno.mkdir(dirname(cacheFile), { recursive: true });
  await Deno.writeTextFile(cacheFile, NormalizedInput, { create: true });

  return (NormalizedInput);
}

async function getPuzzleFromCacheOrRedownload(): Promise<string> {
  const temp =
    Deno.env.get("TEMP") ??
    Deno.env.get("TMP") ??
    (() => {
      throw new Error("TEMP and TMP not found in environment.");
    })();

  const cacheDir = join(temp, "advent-of-code");
  const cacheFile = join(cacheDir, "2015-02.dat");
  const url = "https://adventofcode.com/2015/day/2/input";

  try {
    const stats = await Deno.stat(cacheFile);
    
    if (stats.isFile) {
      const text = await Deno.readTextFile(cacheFile);
      return normalizeText(text);
    }
  } catch {
    // no cache found
  }

    return await getPuzzleFromAOC(cacheFile, url);    
}

function normalizeText(text: string): string {
  const normalized = text.replace(/\r\n/g, "\n").trimEnd() + "\n";
  return (normalized);
}

function dirname(p: string): string {
  const idx = p.replace(/\\/g, "/").lastIndexOf("/");
  return (idx === -1 ? "." : p.slice(0, idx));
}

function calcSurfaceArea({ length: l, width: w, height: h }: present_dimensions): number[] {
    const sides = [l * w, w * h, h * l];
    return (sides);
}

if (import.meta.main) {
  try {
    const args = [...Deno.args];

    const inputPath = args[0];
    const puzzle = inputPath
      ? await getPuzzleFromFile(inputPath)
      : await getPuzzleFromCacheOrRedownload();

    const InputData = puzzle.match(/\d+x\d+x\d+\n/g) ?? [ "0x0x0" ];
    
    let totalSurfaceArea: number = 0;
    let totalRibbonFeet: number  = 0;

    for (let i = 0; i < InputData.length; ++i) {

        const PresentDims: string[] = InputData[i].split("x"); //.match(/\d+/g);
        //const PresentDims2: string[] = InputData[i].match(/\d+/g) ?? [];

        const P: present_dimensions = {
            length: parseInt(PresentDims[DimensionIndex.Length]),
            width: parseInt(PresentDims[DimensionIndex.Width]),
            height: parseInt(PresentDims[DimensionIndex.Height]),
        };

        const sidesSA: number[] = calcSurfaceArea(P);
  
        let surfaceArea = 2 * sidesSA.reduce((sum, side) => sum + side, 0); // reduce(accumulator, iteration value)
        surfaceArea += Math.min(...sidesSA); // add slack of the smallest side's surface area.
  
        totalSurfaceArea += surfaceArea;
        const SortedPresentDims: number[] = getSortedPresentDimensions(P);
  
        const shortPerimeter: number = (2 * SortedPresentDims[0]) + (2 * SortedPresentDims[1]);
  
        const volume: number = P.length * P.width * P.height;
  
        const ribbonFeet: number = volume + shortPerimeter;
  
        totalRibbonFeet += ribbonFeet;
        //console.log('For present ' + (i+1) + ', ribbon need: ' + ribbonFeet);
        //console.log('volume: ' + volume);
        //console.log('shortPerimeter: ' + shortPerimeter);
}

console.log('total surface area is ' + totalSurfaceArea + ' sq ft.');
console.log('total ribbon needed: ' + totalRibbonFeet);

  } catch (e) {
    console.error(`Error: ${e instanceof Error ? e.message : String(e)}`);
    Deno.exit(1);
  }
}