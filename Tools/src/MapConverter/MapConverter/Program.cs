using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace MapConverter
{
    class Program
    {



        static void ConvertMap(string inputFile)
        {
            Dictionary<char, int> mapping = new Dictionary<char, int>
            {
                { '#', 0 },
                { ' ', 1 },

                { '-', 2 },
                { '|', 3 },

                { '@', 4 },

                { '1', 5 },
                { '2', 7 },

                { 'E', 6 },
                { 'M', 8 },

                { '!', 9 },
                { '<', 10 },
                { 'o', 11 },
                { 'x', 12 },
                { 'D', 14 },
                { 'S', 15 },
                { '+', 16 },
            };

            List<int> mapData = new List<int>();

            string levelName = Path.GetFileNameWithoutExtension(inputFile);

            string outputFile = inputFile + ".c";

            using (StreamWriter file = File.CreateText(outputFile))
            {
                string[] lines = File.ReadAllLines(inputFile);


                file.WriteLine($"const MapDefinition Level{levelName}=");
                file.WriteLine("{");

                // name is on first line
                string name = lines[0];
                file.WriteLine($"\t\"{name}\",");
                file.WriteLine("\t{");

                // map data is the next 32 lines
                for (int i=1;i<lines.Length -1;i++)
                {
                    file.Write($"\t\t");
                    string line = lines[i];
                    foreach (char letter in line)
                    {
                        if (mapping.ContainsKey(letter))
                        {
                            int number = mapping[letter];
                            mapData.Add(number);
                            string num = number.ToString().PadRight(2);
                            file.Write(num+",");
                        }
                        else
                        {
                            throw new Exception($"Don't have a match for {letter}");
                        }
                    }
                    file.WriteLine();
                }
                file.WriteLine("\t},");

                //file.BaseStream.Seek(file.BaseStream.Length -1, SeekOrigin.Begin);

                // solution on the last line

                file.WriteLine("\t{");
                file.Write($"\t\t");
                foreach (char letter in lines.Last())
                {
                    file.Write("'{0}',", letter);
                }

                //file.BaseStream.Seek(file.BaseStream.Length - 1, SeekOrigin.Begin);
                file.WriteLine();
                file.WriteLine("\t}");
                file.WriteLine("};");
            }




            var rle = RunLengthEncoding(mapData);
            Console.WriteLine($"input size {mapData.Count} RLE: {rle.Count}  max {rle.Max()}");
        }


        static List<int> RunLengthEncoding(List<int> input)
        {
            List<int> output = new List<int>();

            int current = input[0];
            int count = 1;

            for (int i = 1; i < input.Count; i++)
            {
                if (input[i] != current)
                {
                    output.Add(count);
                    output.Add(current);

                    if (i + 1 < input.Count)
                    {
                        current = input[i + 1];
                        count = 0;
                    }
                }
                else
                {
                    count++;
                }
            }


            return output;
        }


        public static void GenerateGameMapsC(string path)
        {
            var files = Directory.GetFiles(path).Where(x => x.EndsWith(".txt.c")).OrderBy(c => int.Parse(Regex.Match(c, @"\d+").Value)).ToList();

            using (StreamWriter fileWriter = File.CreateText(Path.Combine(path, "GameMaps.c")))
            {
                fileWriter.WriteLine(File.ReadAllText("GameMaps.template"));

                foreach (var file in files)
                {
                    fileWriter.WriteLine(File.ReadAllText(file));
                    fileWriter.WriteLine();
                }

                fileWriter.WriteLine("const MapDefinition* GameMaps[MAP_COUNT] =");
                fileWriter.WriteLine("{");
                foreach (var file in files)
                {
                    var name = Path.GetFileName(file);
                    name = name.Substring(0, name.Length - 6);
                    fileWriter.WriteLine($"\t&Level{name},");
                }
                fileWriter.WriteLine("};");
            }
        }
        

        static void Main(string[] args)
        {
            string inputFile = args[0];
            string cFile = null;
            if (args.Length >= 2)
            {
                cFile = args[1];
            }

            FileAttributes attr = File.GetAttributes(inputFile);

            //detect whether its a directory or file
            if ((attr & FileAttributes.Directory) == FileAttributes.Directory)
            {
                var files = Directory.GetFiles(inputFile).Where(x => x.EndsWith(".txt")).OrderBy(c => int.Parse( Regex.Match(c, @"\d+").Value ) ).ToList();

                foreach (var file in files)
                {
                    try
                    {
                        ConvertMap(file);
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine($"Failed to convert file: {file} {ex}");
                        break;
                    }

                    Console.WriteLine($"Processed {file}");
                }



                GenerateGameMapsC(inputFile);

            }
            else
            {
                ConvertMap(inputFile);
            }
        }
    }
}
