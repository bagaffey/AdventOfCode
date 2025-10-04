// deno run Day4.ts [secretkey]
// This requires a node package..
import { createHash } from "node:crypto";

function md5Hex(input: string): string {
    return createHash("md5").update(input).digest("hex");
 }

function solve(secretkey: string, prefix: string): number {
    let n = 0;
    
    for (;;) {
        ++n;
        const result = md5Hex(secretkey + n);
        if (result.startsWith(prefix))
            return (n);
    }
}

if (import.meta.main) {
  try {
    const SecretKey: string = Deno.args[0];
    if (!SecretKey) throw new Error("Usage: deno run Day4.ts <secretKey>");

    const part1Solution = await solve(SecretKey, "00000");
    console.log(`Part 1 solution is: ${part1Solution}`);

    const part2Solution = await solve(SecretKey, "000000");
    console.log(`Part 2 solution is: ${part2Solution}`);

  } catch (e) {
    console.error(`Error: ${e instanceof Error ? e.message : String(e)}`);
    Deno.exit(1);
  }
}