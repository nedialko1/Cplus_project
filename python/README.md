# Python *Baseline*: The Prototype Scripts

> **Context:**  The core algorithms are laid out here. Being free from strong type constraints, memory allocation, or compilation barriers, Python provides rapidly readable reference implementations. The latter sacrifice computing control for expressive freedom.

---

## Key Language-Specific Architectural Characteristics

* **Typing Model:** Dynamic and implicit. Variables require no declarations; containers accept arbitrary objects governed by Python's runtime object model.
* **Heap Abstraction:** Python lacks a native standalone binary heap class. 
For instance, the `heapq` module provides procedural min-heap functions operating on standard dynamic lists (`list`). Max-heap behavior is simulated via explicit value negation $\text{-num}$.
* **Returning tuples:** Implicit tuple allocation and unpacking (`return running_val, elapsed_ms`), relying on dynamic object packing.

---

# 1. The **`streaming_stats`** app

## Pseudocode: 
* **Purpose:** Compute the running median of a data stream in 
$O(\log~n)$ 
time per element using a balanced two-heap architecture.
* **Input:** A sequential data stream 
```math
\text{data\_stream} = [ x_1, x_2, \dots, x_n ]
```
where each ingested value $\text{num} \in \mathbb{R}$.
* **Output:** Per-element return tuple 
```math
(\text{running\_val}, \text{elapsed\_ms}) \in \mathbb{R} \times \mathbb{R}^+$
```

### State Initialization

```math 
\begin{matrix}

\text{max\_heap} \leftarrow []
\\
\text{min\_heap} \leftarrow []

\end{matrix}
```

### Step 1: Heap Selection & Insertion

For an incoming value $\text{num}$:

```math 
\begin{matrix}

\text{if } \neg\text{max\_heap} \lor \text{num} \le -\text{max\_heap}[0] \implies \text{heappush}(\text{max\_heap}, -\text{num})
\\
\text{else} \implies \text{heappush}(\text{min\_heap}, \text{num})

\end{matrix}
``` 

### Step 2: Size Invariant & Rebalancing

Maintain invariant 

```math
\begin{matrix}

|\text{max\_heap}| \in \{|\text{min\_heap}|, |\text{min\_heap}| + 1\}:
\\
\text{if } \text{len}(\text{max\_heap}) > \text{len}(\text{min\_heap}) + 1 \implies \text{val} \leftarrow -\text{heappop}(\text{max\_heap}), \; \text{heappush}(\text{min\_heap}, \text{val})
\\
\text{elif } \text{len}(\text{min\_heap}) > \text{len}(\text{max\_heap}) \implies \text{val} \leftarrow 
\text{heappop}(\text{min\_heap}), \; \text{heappush}(\text{max\_heap}, -\text{val})

\end{matrix}
``` 

### Step 3: Running Statistic Computation

```math

\begin{matrix}

\text{running\_val} = \frac{-\text{max\_heap}[0] + \text{min\_heap}[0]}{2.0} \quad (\text{if sizes are equal})
\\
\text{running\_val} = -\text{max\_heap}[0] \quad (\text{otherwise})

\end{matrix}
```

### Step 4: Timing, Return, and Stream Loop

```math
\begin{matrix}

\text{elapsed\_ms} = (\text{perf\_counter}() - t_{\text{start}}) \times 1000.0
\\
\text{return } (\text{running\_val}, \text{elapsed\_ms})
\\
\forall x_i \in \text{data\_stream}: (\text{res}, \text{t\_ms}) \leftarrow \text{insert}(x_i)

\end{matrix}
``` 
---

## Flowchart

### **Color-Coding Guide (Anticipating matches to C++ Evolution)**

🟦 **Blue Nodes (Important Facilitatory Paradigm):** 
* *Role:* Architectural patterns and STL container adapters (e.g., standard `std::priority_queue` configuration, underlying vector storage).

🟩 **Green Nodes (Progress with Memory Safety & Compile-Time Validation):** 
* *Role:* Modern C++ features (C++20/23 Concepts, type constraints, move semantics safety, and compile-time validation).

🟥 **Red Nodes (Critical Shortcomings / Workarounds):** 
* *Role:* Legacy boilerplates, manual memory management traps, or custom comparator workarounds that higher C++ standards eliminate.

🟨 **Yellow Nodes (Usual Stuff / Standard Operational Logic):** 
* *Role:* Baseline procedural execution steps (size checks, standard element swaps, invariant branch checks).

```mermaid
graph TD
    classDef safety fill:#E8F5E9,stroke:#2E7D32,stroke-width:2px,color:#00FF00,font-size:13px,font-weight:bold;
    classDef paradigm fill:#E3F2FD,stroke:#1565C0,stroke-width:2px,color:#0000FF,font-size:13px,font-weight:bold;
    classDef shortcoming fill:#FFEBEE,stroke:#C62828,stroke-width:2px,color:#FF0000,font-size:13px,font-weight:bold;
    classDef usual fill:#FFFDE7,stroke:#FBC02D,stroke-width:2px,color:#0000FF,font-size:13px,font-weight:bold;
    classDef footer fill:#EAEAEA,stroke:#BDBDBD,stroke-width:1px,color:#000000,font-size:14px,font-weight:bold;

    subgraph Ingestion [" "]
        A([Incoming Stream Value: num]):::paradigm --> B{"max_heap empty OR<br/>num <= -max_heap[0]?"}:::usual
        
        B -- "Yes" --> C[("heappush(max_heap, -num)")]:::shortcoming
        B -- "No" --> D[("heappush(min_heap, num)")]:::usual
        
        Sub1_Label["1. Stream Ingestion & Heap Routing"]:::footer
    end

    subgraph Rebalance [" "]
        C --> E
        D --> E{"|max_heap| > |min_heap| + 1?"}:::usual
        
        E -- "Yes" --> F["val = -heappop(max_heap)<br/>heappush(min_heap, val)"]:::shortcoming
        E -- "No" --> G{"|min_heap| > |max_heap|?"}:::usual
        
        F --> H["Invariant Satisfied"]:::usual
        G -- "Yes" --> I["val = heappop(min_heap)<br/>heappush(max_heap, -val)"]:::shortcoming
        G -- "No" --> H
        I --> H
        
        Sub2_Label["2. Size Invariant Rebalancing"]:::footer
    end

    subgraph Extraction [" "]
        H --> J{"|max_heap| == |min_heap|?"}:::usual
        
        J -- "Yes" --> K["Median = (-max_top + min_top) / 2.0"]:::usual
        J -- "No" --> L["Median = -max_top"]:::usual
        
        K --> M([Return Tuple: running_val, elapsed_ms]):::paradigm
        L --> M
        
        Sub3_Label["3. Median Extraction & Return"]:::footer
    end

    style Ingestion fill:#F9F9F9,stroke:#BDBDBD,stroke-width:2px;
    style Rebalance fill:#F9F9F9,stroke:#BDBDBD,stroke-width:2px;
    style Extraction fill:#F9F9F9,stroke:#BDBDBD,stroke-width:2px;
```    
