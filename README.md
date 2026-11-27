# Farm-to-Market Route Planner

A C++ console program that uses graph algorithms to plan how farm produce can travel from different places (upazilas) to one main wholesale market. The road network is read from a text file, so the program can work with the data of any area. A sample dataset of **Chandpur District** is included.

This project was made for **CSE 208 – Data Structures and Algorithms Lab II**, Department of CSE, University of Asia Pacific.

## Problem

Farmers in different places must send their produce to one main market. Roads have different lengths, and a flood or a broken bridge can block a road. Planners need quick answers to these questions:

- Which route to the market is the shortest? Which route has the fewest road segments?
- If one road is blocked, which places can no longer reach the market?
- Which roads connect every place with the least total length?

## Features

The program runs from a console menu:

| Option | Feature | Algorithm |
|--------|---------|-----------|
| 1 | Show the places and roads | – |
| 2 | Shortest route (in km) from one place to another, with the distance to every place | Dijkstra |
| 3 | Route with the fewest road segments | BFS |
| 4 | Blocked road check: which places are cut off from the market | DFS |
| 5 | Minimum road network that connects all places | Prim |
| 6 | Minimum road network that connects all places | Kruskal |
| 7 | Best truck load: which products to carry for the highest profit within the truck capacity (products can be taken in part) | Fractional Knapsack (Greedy) |
| 0 | Exit | – |

Other points:

- The road network is an undirected weighted graph stored as an adjacency matrix.
- Wrong keyboard input and invalid data files are handled (for example, a missing file, a road that joins a place to itself, or a distance of 0 or less).
- The blocked road check does not change the network, so later menu choices still use the full data.
- Option 7 reads a separate product file and does not use the road network.

## Algorithms and Complexity

`V` is the number of places and `E` is the number of roads.

| Algorithm | Time complexity in this program |
|-----------|---------------------------------|
| Dijkstra | O(V²) |
| BFS | O(V²) |
| DFS | O(V²) |
| Prim | O(V²) |
| Kruskal | O(E²), because bubble sort is used to sort the roads |
| Fractional Knapsack | O(P²), because bubble sort is used to sort the products (`P` is the number of products) |

> **Note:** The report and the presentation in the `docs/` folder were written for the first version of the project (menu options 1 to 6). Option 7 (Fractional Knapsack) was added later and is **not** described in them. The complexity of option 7 above comes from reading the code.

The full explanation, pseudocode and analysis of options 1 to 6 are in the report in the `docs/` folder.

## Technologies Used

- **Language:** C++
- **IDE:** Code::Blocks
- **Operating system:** Windows 10
- **Input:** a text file, read with `ifstream`
- **Header files:** `iostream`, `fstream`, `string`, `algorithm`
- **Coding style:** plain arrays, functions and recursion (no `struct`, `class` or `vector`)

## Project Structure

```
farm-to-market-route-planner/
├── README.md
├── src/
│   └── route_planner.cpp
├── data/
│   ├── chandpur_data.txt
│   └── products_data.txt
└── docs/
    ├── Farm_to_Market_Route_Planner_Report.pdf
    └── Farm_to_Market_Route_Planner.pptx
```

## How to Compile and Run

You need a C++ compiler such as g++.

From the main folder of the project, compile:

```
g++ src/route_planner.cpp -o route_planner
```

Run:

```
./route_planner
```

On Windows, run `route_planner.exe` instead.

When the program asks for the data file name, type:

```
data/chandpur_data.txt
```

Then choose an option from the menu. If you choose option 7, the program also asks for the product file name. Type:

```
data/products_data.txt
```

## Data File Format

The program reads a plain text file in this format:

```
n
name of place 0
name of place 1
...
name of place n-1
m
a b km
a b km
...
```

- `n` is the number of places (between 2 and 20).
- Place names are written without spaces (use `_`, for example `Chandpur_Sadar`).
- `m` is the number of roads. Each road line has two place numbers (starting from 0) and the distance in km.
- Place number `0` is always treated as the **main market**.

To use another area, create a new file in this format and type its name when the program asks.

## Product Data File Format (option 7)

Option 7 reads a second text file in this format:

```
truck capacity in kg
number of products
name weight_in_kg total_profit
name weight_in_kg total_profit
...
```

- The capacity must be more than 0.
- The number of products is between 1 and 20.
- The weight and the profit of each product must be more than 0. The profit is the total profit of the whole weight of that product.
- Product names are written without spaces.

The file `data/products_data.txt` has 8 products and a truck capacity of 150 kg.

> The weights and profits are demonstration values. They are not real market prices.

## Sample Dataset: Chandpur District

The file `data/chandpur_data.txt` has 8 upazilas of Chandpur District and 13 roads. Place 0 (`Chandpur_Sadar`) is the main market.

> The distances are approximate and used for demonstration only. They have not been checked on a map.

### Sample results

| Test | Input | Result |
|------|-------|--------|
| Dijkstra | From 7 (Shahrasti) to 0 (Chandpur_Sadar) | Shahrasti -> Chandpur_Sadar, 38 km |
| BFS | From 7 (Shahrasti) to 3 (Matlab_Uttar) | Shahrasti -> Chandpur_Sadar -> Matlab_Dakshin -> Matlab_Uttar, 3 road segments |
| DFS | Block road 2 - 3 (Matlab_Dakshin - Matlab_Uttar) | Cut off from market: Matlab_Uttar |
| Prim | – | Total length: 126 km (7 roads) |
| Kruskal | – | Total length: 126 km (7 roads) |

Prim and Kruskal choose the same 7 roads in a different order, so both give 126 km.

### Sample result for option 7 (Fractional Knapsack)

Input file: `data/products_data.txt` (truck capacity 150 kg, 8 products).

The program sorts the products by profit per kg (Rice 20, Mango 19, Tomato 18, Onion 17, Jute 15, Brinjal 13, Potato 12, Banana 11 taka per kg) and loads the best ones first:

| Product | Loaded | Profit (taka) |
|---------|--------|---------------|
| Rice | 40 kg (full) | 800 |
| Mango | 35 kg (full) | 665 |
| Tomato | 25 kg (full) | 450 |
| Onion | 20 kg (full) | 340 |
| Jute | 30 kg (part of 50 kg) | 450 |

Truck load: 150 kg. **Total profit: 2705 taka.**

## Limitations

- At most 20 places and at most 20 products (the arrays have a fixed size).
- Place 0 is always the main market.
- Only distance in km is used. Traffic, road condition and river routes are not included.
- The blocked road check handles one road at a time.
- Only one route is printed, even if two routes are equally good.
- Option 7 assumes products can be taken in part (for example, by kg). It does not handle products that must be taken whole.
- Option 7 is separate from the road network. It does not connect the truck load with the routes.
- Kruskal and Fractional Knapsack use bubble sort, so they are slow for large inputs.
- The adjacency matrix needs V × V memory.

## Future Work

- Use distances checked on a map.
- Add travel time and flood data.
- Include villages in the network.
- Use a faster sort and a priority queue for large networks.

## Documentation

- [Project report (PDF)](docs/Farm_to_Market_Route_Planner_Report.pdf)
- [Presentation (PPTX)](docs/Farm_to_Market_Route_Planner.pptx)
