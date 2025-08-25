using System.Net;
using System.Net.Http;
using System.ComponentModel;

class Program
{
    private static List<Int32> GetMassesInputFromFile(string InputDataFilePath)
    {
        if (!File.Exists(InputDataFilePath))
            throw new FileNotFoundException($"Input file not found: {InputDataFilePath}");

        List<Int32> Results = new List<Int32>();

        foreach (String L in File.ReadLines(InputDataFilePath))
        {
            string S = L.Trim();
            if (string.IsNullOrEmpty(S)) continue;
            if (!Int32.TryParse(S, out Int32 V))
                throw new FormatException($"Invalid integer in input data file: '${L}'");
            Results.Add(V);
        }

        return (Results);
    }

    public static List<Int32> GetMassesInputFromCacheOrRedownload()
    {
        string CacheDir = Path.GetTempPath();
        string CacheFile = Path.Combine(CacheDir, "2019-12-01.dat");

        if (File.Exists(CacheFile))
        {
            return GetMassesInputFromFile(CacheFile);
        }
        else
        {
            return GetInputMassesFromAOC(CacheFile);
        }
    }

    public static List<Int32> GetInputMassesFromAOC(string DataCacheFile)
    {
        string? AOCSessionCooke = Environment.GetEnvironmentVariable("AOC_SESSION");

        if (string.IsNullOrWhiteSpace(AOCSessionCooke))
            throw new InvalidOperationException("AOC_SESSION environment variable is not set. Provide a file path or set AOC_SESSION.");

        HttpClientHandler H = new HttpClientHandler
        {
            UseCookies = true,
            CookieContainer = new CookieContainer()
        };

        Uri U = new Uri("https://adventofcode.com/2019/day/1/input");
        H.CookieContainer.Add(new Uri("https://adventofcode.com"), new Cookie("session", AOCSessionCooke));

        using HttpClient http = new HttpClient(H);
        using HttpResponseMessage? Resp = http.GetAsync(U).GetAwaiter().GetResult();
        if (!Resp.IsSuccessStatusCode)
        {
            string B = Resp.Content.ReadAsStringAsync().GetAwaiter().GetResult();
            throw new InvalidOperationException($"Failed to fetch input ({(int)Resp.StatusCode} {Resp.ReasonPhrase}). Body: {B}");
        }

        string Content = Resp.Content.ReadAsStringAsync().GetAwaiter().GetResult();
        File.WriteAllText(DataCacheFile, Content);
        return (GetMassesInputFromFile(DataCacheFile));
    }

    internal static Int32 CalcFuelRequirement(Int32 ModuleMass)
    {
        Int32 Result = 0;
        Result = (ModuleMass / 3) - 2;
        return (Result);
    }

    internal static Int32 CalcRealFuelRequirement(Int32 ModuleMass)
    {
        Int32 Result = 0;
        Result = (ModuleMass / 3) - 2;
        if (Result <= 0)
        {
            return (0);
        }
        else
        {
            Result += CalcRealFuelRequirement(Result);
            return (Result);
        }
    }

    public static int Main(string[] args)
    {
        string? InputPath = args.Length > 0 ? args[0] : null;

        List<Int32> ModuleMasses;

        if (!String.IsNullOrWhiteSpace(InputPath))
        {
            ModuleMasses = GetMassesInputFromFile(InputPath);
        }
        else
        {
            LoadEnv("..\\..\\..\\..\\..\\..\\..\\.env");
            ModuleMasses = GetMassesInputFromCacheOrRedownload();
        }

        Int32 TotalModulesFuel = ModuleMasses.Sum(CalcFuelRequirement);
        Console.WriteLine($"\n(Part 1) - Transport masses fuel required: {TotalModulesFuel}");

        Int32 TotalTripFuelRequirement = ModuleMasses.Sum(CalcRealFuelRequirement);
        Console.WriteLine($"\n(Part 2) - Total Fuel for Trip Required: {TotalTripFuelRequirement}");

        return (0);
    }

    static void LoadEnv(string EnvFilePath)
    {
        if (!File.Exists(EnvFilePath)) return;

        foreach (string L in File.ReadAllLines(EnvFilePath))
        {
            // Skip comments or empty lines
            if (string.IsNullOrWhiteSpace(L) || L.TrimStart().StartsWith("#"))
                continue;

            string[] ParsedString = L.Split('=', 2); // split at first '='
            if (ParsedString.Length != 2) continue;

            string Key = ParsedString[0].Trim();
            string Value = ParsedString[1].Trim().Trim('"'); // strip surrounding quotes

            Environment.SetEnvironmentVariable(Key, Value, EnvironmentVariableTarget.Process);
        }
    }
}