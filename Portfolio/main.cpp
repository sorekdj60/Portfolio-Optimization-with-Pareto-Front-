#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <limits>
#include <set>
#include <numeric>
using namespace std;

class Portfolio {
public:
    vector<double> allocations;  // Weights for asset allocation
    double netReturn;  // Net return after transaction costs
    double volatility;  // Portfolio risk
    double transactionCost;  // Cost incurred due to transactions

    Portfolio(int numAssets = 0) {
        allocations.resize(numAssets);
        netReturn = 0.0;
        volatility = 0.0;
        transactionCost = 0.0;
    }

    // Evaluate portfolio's performance based on expected returns and risk
    void evaluate(const vector<double>& expectedReturns, const vector<vector<double>>& covarianceMatrix, double transactionCostRate) {
        double totalReturn = 0.0;
        double totalRisk = 0.0;

        // Calculate expected return of the portfolio
        for (size_t i = 0; i < allocations.size(); i++) {
            totalReturn += allocations[i] * expectedReturns[i];
        }

        // Calculate risk (volatility) of the portfolio
        for (size_t i = 0; i < allocations.size(); i++) {
            for (size_t j = 0; j < allocations.size(); j++) {
                totalRisk += allocations[i] * allocations[j] * covarianceMatrix[i][j];
            }
        }

        // Calculate net return after accounting for transaction costs
        netReturn = totalReturn - transactionCost;
        volatility = sqrt(totalRisk);  // Risk is the square root of variance
        transactionCost = transactionCostRate * accumulate(allocations.begin(), allocations.end(), 0.0);  // Transaction cost is proportional to total allocation
    }

    // Determine if this portfolio dominates another (higher return with lower risk)
    bool dominates(const Portfolio& other) const {
        return (netReturn > other.netReturn && volatility < other.volatility) ||
               (netReturn >= other.netReturn && volatility <= other.volatility && (netReturn > other.netReturn || volatility < other.volatility));
    }

    // Overload the < operator to allow sorting in a set
    bool operator<(const Portfolio& other) const {
        if (netReturn != other.netReturn) {
            return netReturn < other.netReturn;
        }
        return volatility < other.volatility;
    }
};

class PortfolioSimulation {
private:
    int numAssets;  // Number of assets in the portfolio
    int numSimulations;  // Number of simulations to run
    double transactionCostRate;  // Transaction cost rate
    int minAssets;  // Minimum number of assets in a portfolio
    int maxAssets;  // Maximum number of assets in a portfolio
    vector<double> expectedReturns;  // Expected returns of individual assets
    vector<vector<double>> covarianceMatrix;  // Covariance matrix representing the risk of the assets

public:
    PortfolioSimulation(int numAssets, int numSimulations, double transactionCostRate, int minAssets, int maxAssets,
                        const vector<double>& expectedReturns, const vector<vector<double>>& covarianceMatrix)
        : numAssets(numAssets), numSimulations(numSimulations), transactionCostRate(transactionCostRate),
          minAssets(minAssets), maxAssets(maxAssets), expectedReturns(expectedReturns), covarianceMatrix(covarianceMatrix) {}

    // Generate a random portfolio by assigning random allocations to assets
    Portfolio generateRandomPortfolio() const {
        Portfolio portfolio(numAssets);
        double totalWeight = 0.0;
        for (int i = 0; i < numAssets; i++) {
            portfolio.allocations[i] = rand() % 100;
            totalWeight += portfolio.allocations[i];
        }
        for (int i = 0; i < numAssets; i++) {
            portfolio.allocations[i] /= totalWeight;
        }
        portfolio.transactionCost = totalWeight * transactionCostRate;
        return portfolio;
    }

    // Simulate a population of portfolios, applying constraints
    vector<Portfolio> simulatePortfolios() const {
        vector<Portfolio> population;

        for (int i = 0; i < numSimulations; i++) {
            Portfolio portfolio = generateRandomPortfolio();
            portfolio.evaluate(expectedReturns, covarianceMatrix, transactionCostRate);

            // Check if the number of assets in the portfolio is within the constraints
            int activeAssets = count_if(portfolio.allocations.begin(), portfolio.allocations.end(), [](double weight) { return weight > 0.01; });
            if (activeAssets >= minAssets && activeAssets <= maxAssets) {
                population.push_back(portfolio);
            }
        }

        return population;
    }

    // Construct the Pareto front from a population of portfolios
    set<Portfolio> constructParetoFront(const vector<Portfolio>& population) const {
        set<Portfolio> paretoFront;

        for (const Portfolio& portfolio : population) {
            bool isDominated = false;
            auto it = paretoFront.begin();

            while (it != paretoFront.end()) {
                if (it->dominates(portfolio)) {
                    isDominated = true;
                    break;
                }
                if (portfolio.dominates(*it)) {
                    it = paretoFront.erase(it);
                } else {
                    ++it;
                }
            }

            if (!isDominated) {
                paretoFront.insert(portfolio);
            }
        }

        return paretoFront;
    }
};

int main() {
    srand(time(0));  // Seed the random number generator

    int numAssets = 5;  // Number of assets in the portfolio
    int numSimulations = 10000;  // Number of portfolios to simulate
    double transactionCostRate = 0.001;  // Transaction cost rate (0.1%)
    int minAssets = 2;  // Minimum number of assets allowed in a portfolio
    int maxAssets = 4;  // Maximum number of assets allowed in a portfolio

    // Example data: expected returns and covariance matrix for the assets
    vector<double> expectedReturns = {0.12, 0.10, 0.14, 0.08, 0.11};  // Expected returns for the assets
    vector<vector<double>> covarianceMatrix = {  // Covariance matrix representing the risk
        {0.1, 0.02, 0.04, 0.01, 0.03},
        {0.02, 0.15, 0.05, 0.02, 0.01},
        {0.04, 0.05, 0.2, 0.01, 0.02},
        {0.01, 0.02, 0.01, 0.3, 0.01},
        {0.03, 0.01, 0.02, 0.01, 0.25}
    };

    PortfolioSimulation simulation(numAssets, numSimulations, transactionCostRate, minAssets, maxAssets, expectedReturns, covarianceMatrix);

    // Simulate portfolios with constraints using Monte Carlo
    vector<Portfolio> population = simulation.simulatePortfolios();

    set<Portfolio> paretoFront = simulation.constructParetoFront(population);

    cout << "Pareto Front:\n";
    for (const Portfolio& portfolio : paretoFront) {
        cout << "Return: " << portfolio.netReturn << ", Risk: " << portfolio.volatility << ", Transaction Cost: " << portfolio.transactionCost << "\n";
    }

    return 0;
}
