// deno run --allow-env=AOC_SESSION,TEMP,TMP --allow-read --allow-write --allow-net=adventofcode.com Day2.ts [inputFile]

import { join } from "https://deno.land/std/path/mod.ts";

type house_telemetry = {
    HousesCount: string[],
    DistinctHouseCount: string[],
}

type entity_position = {
    X: number,
    Y: number,
}

function PrintPosition(P: entity_position): string {
    return (`${P.X}, ${P.Y}`);
}

function ProcessMovement(EP: entity_position, token: string): void {
    switch (token) {
        case 'v':
            --EP.Y;
            break;
        case '>':
            ++EP.X;
            break;
        case '<':
            --EP.X;
            break;
        case '^':
            ++EP.Y;
            break;
        default:
            console.log(`Unexpected char: ${token}`)
            break;
    }
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
  const url = "https://adventofcode.com/2015/day/3/input";

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

    let SantaPosition: entity_position = { X: 0, Y: 0, };
    let RoboSantaPosition: entity_position = { X: 0, Y: 0, };
    let NoHelpSantaPosition: entity_position = { X: 0, Y: 0, };

    let isSantaTurn = true;

    let NeighborhoodInfo: house_telemetry = {
        DistinctHouseCount: [],
        HousesCount: [],
    };
    NeighborhoodInfo.DistinctHouseCount.push(PrintPosition(SantaPosition));
    NeighborhoodInfo.HousesCount.push(PrintPosition(SantaPosition));

    for (let i = 0; i < InputData.length; ++i) {
        const token: string = InputData.charAt(i);

        if (isSantaTurn)
            ProcessMovement(SantaPosition, token);
        else
            ProcessMovement(RoboSantaPosition, token);

        ProcessMovement(NoHelpSantaPosition, token);

        if (isSantaTurn) {
            isSantaTurn = false;
        } else {
            isSantaTurn = true;
        }
    }
  } catch (e) {
    console.error(`Error: ${e instanceof Error ? e.message : String(e)}`);
    Deno.exit(1);
  }
}