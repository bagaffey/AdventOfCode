// deno run --allow-env=AOC_SESSION,TEMP,TMP --allow-read --allow-write --allow-net=adventofcode.com Day4.ts [inputFile]

import { join } from "https://deno.land/std/path/mod.ts";

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
  const cacheFile = join(cacheDir, "2015-04.dat");
  const url = "https://adventofcode.com/2015/day/4/input";

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

if (import.meta.main) {
  try {
    const args = [...Deno.args];

    const inputPath = args[0];
    const PuzzleInput = inputPath
      ? await getPuzzleFromFile(inputPath)
      : await getPuzzleFromCacheOrRedownload();

    const InputData: string = PuzzleInput;

  } catch (e) {
    console.error(`Error: ${e instanceof Error ? e.message : String(e)}`);
    Deno.exit(1);
  }
}