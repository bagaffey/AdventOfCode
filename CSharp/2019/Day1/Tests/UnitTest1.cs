using System;
using System.IO;
using System.Linq;
using Xunit;

namespace AOC._2019.Day01.Tests
{

    public class Day1Tests
    {
        // Examples from Part 1
        [Theory]
        [InlineData(12, 2)]
        [InlineData(14, 2)]
        [InlineData(1969, 654)]
        [InlineData(100756, 33583)]
        public void CalcFuelRequirement_matches_examples(int mass, int expected)
        {
            int actual = Program.CalcFuelRequirement(mass);
            Assert.Equal(expected, actual);
        }

        [Fact]
        public void Part1_sum_of_examples_is_34241()
        {
            int[] masses = new[] { 12, 14, 1969, 100756 };
            int total = masses.Sum(Program.CalcFuelRequirement);
            Assert.Equal(34241, total);
        }

        // Examples from Part 2
        [Theory]
        [InlineData(12, 2)]
        [InlineData(14, 2)]
        [InlineData(1969, 966)]
        [InlineData(100756, 50346)]
        public void CalcRealFuelRequirement_matches_examples(int mass, int expected)
        {
            int actual = Program.CalcRealFuelRequirement(mass);
            Assert.Equal(expected, actual);
        }

        [Fact]
        public void Part2_sum_of_examples_is_51316()
        {
            int[] masses2 = [12, 14, 1969, 100756];
            int[] masses = new[] { 12, 14, 1969, 100756 };
            int total = masses.Sum(Program.CalcRealFuelRequirement);
            Assert.Equal(51316, total);
        }
    }

    // A tiny end-to-end (integration) test that runs Program.Main with a temp file.
    public class Day1EndToEndTests
    {
        [Fact]
        public void Main_with_sample_file_prints_expected_totals_and_returns_zero()
        {
            // Arrange: write the 4 example masses to a temp file
            string path = Path.GetTempFileName();
            File.WriteAllText(path, "12\n14\n1969\n100756\n");

            StringWriter sw = new StringWriter();
            TextWriter originalOut = Console.Out;
            Console.SetOut(sw);

            try
            {
                // Act: run the app with the file path argument.
                int rc = Program.Main(new[] { path });

                // Assert: exit code and printed totals
                Assert.Equal(0, rc);
                string output = sw.ToString();
                Assert.Contains("(Part 1)", output);
                Assert.Contains("34241", output);
                Assert.Contains("(Part 2)", output);
                Assert.Contains("51316", output);
            }
            finally
            {
                Console.SetOut(originalOut);
                try { File.Delete(path); } catch { /* ignore the deletion of a temporary file */ }
            }
        }

        [Fact]
        public void Main_with_bad_input_throws_FormatException()
        {
            // Arrange: write a bad line
            string path = Path.GetTempFileName();
            File.WriteAllText(path, "12\noops\n");

            try
            {
                // Act + Assert: Program should propagate the FormatException from parsing
                Assert.Throws<FormatException>(() => Program.Main(new[] { path }));
            }
            finally
            {
                try { File.Delete(path); } catch { /* ignore */ }
            }
        }
    }
}