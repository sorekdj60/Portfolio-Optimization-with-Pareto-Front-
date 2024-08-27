

# Portfolio Optimization with Pareto Front


This project aims to solve a classic problem in finance: **portfolio optimization**. The objective is to generate investment portfolios that maximize return while minimizing risk. The problem is tackled using simulation techniques and multi-objective optimization through the construction of the **Pareto front**.

## Problem to Solve

In finance, an investor typically seeks to maximize returns while minimizing the risk of their portfolio. However, these two objectives are often in conflict: increasing returns usually involves taking on more risk.

### Objectives

1. **Maximize return**: We aim to identify portfolios with the highest possible returns.
2. **Minimize risk**: We also strive to reduce portfolio risk (measured as volatility).
3. **Find optimal portfolios**: The final goal is to find the **Pareto front**, which represents the set of non-dominated portfolios, offering the best trade-offs between return and risk.

### Constraints

- Each portfolio must contain between 2 and 4 assets (stocks).
- Transaction costs are applied based on the volume of transactions.
- The sum of the asset weights (the percentage of the portfolio allocated to each stock) must always equal 100%.

## Approach and Solution

The problem was solved using a **Monte Carlo simulation** to generate a large number of random portfolios, followed by constructing a **Pareto front** to identify the most optimal portfolios.

### Steps to Solve

1. **Portfolio Simulation**:
   - Random portfolios are generated with normalized weights to ensure that the sum of the weights equals 1.
   - For each portfolio, a number of assets (between 2 and 4) is selected.

2. **Portfolio Evaluation**:
   - The **expected return** of each portfolio is calculated based on the asset weights and their expected individual returns.
   - The **risk** (or volatility) is evaluated using the covariance matrix of asset returns. This is a classic measure of risk in finance based on variance and covariance of assets.
   - A proportional transaction cost is also applied to account for costs associated with changing the asset weights.

3. **Constructing the Pareto Front**:
   - The **Pareto front** is built by comparing the generated portfolios. A portfolio is part of the Pareto front if it is not dominated by any other portfolio in terms of both return and risk. In other words, a portfolio is on the Pareto front if no other portfolio has both a higher return and lower risk.

### Mathematical Model

- **Portfolio Return**:
  \[
  R_{portfolio} = \sum_{i=1}^{n} w_i \cdot R_i
  \]
  where \( w_i \) is the weight of asset \( i \), and \( R_i \) is the expected return of asset \( i \).

- **Portfolio Risk (Variance)**:
  \[
  \sigma^2_{portfolio} = \sum_{i=1}^{n} \sum_{j=1}^{n} w_i \cdot w_j \cdot \text{Cov}(R_i, R_j)
  \]
  where \( \text{Cov}(R_i, R_j) \) is the covariance between the returns of assets \( i \) and \( j \), and \( w_i \) and \( w_j \) are their respective weights.

- **Pareto Front**:
  The Pareto front is a set of portfolios for which no improvement in return can be made without increasing risk, and no reduction in risk can be made without lowering the return.

## Technologies Used

- **C++**: Programming language used to implement portfolio simulations and the construction of the Pareto front.
- **Monte Carlo Algorithms**: To generate a large number of random portfolios with different weights.
- **Multi-Objective Optimization**: Used to select the optimal portfolios for the Pareto front.

## Code Structure

- **`Portfolio`**: This class represents a financial portfolio. It contains methods to evaluate the portfolio's return, risk, and transaction costs.
- **`PortfolioSimulation`**: This class simulates a large number of random portfolios and constructs the Pareto front from the population of generated portfolios.
- **`main()`**: Entry point of the program where the simulation is executed and the results (optimal portfolios) are displayed.

## How to Run

1. Compile the program using a compatible C++ compiler (such as `g++`).
   ```bash
   g++ main.cpp -o portfolio_simulation
   ```

2. Run the program.
   ```bash
   ./portfolio_simulation
   ```

3. The program will display the portfolios on the Pareto front, showing their returns, risks, and transaction costs.

## Example Output

After the simulation is complete, the program will display the Pareto front:

```
Pareto Front:
Return: 0.105, Risk: 0.215, Transaction Cost: 0.002
Return: 0.102, Risk: 0.190, Transaction Cost: 0.001
...
```

These portfolios represent the best trade-offs between return and risk.

## Future Improvements

- **Dynamic Reallocation**: Implement dynamic reallocation strategies to rebalance portfolios according to changing market conditions.
- **Real Historical Data**: Use real historical data to simulate and test the portfolios over multiple years.
- **Stochastic Optimization**: Introduce stochastic optimization methods, such as genetic algorithms, to improve the optimization of the Pareto front.

