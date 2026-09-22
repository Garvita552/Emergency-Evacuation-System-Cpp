#ifndef RISK_ANALYZER_HPP
#define RISK_ANALYZER_HPP

#include "../Simulation/BuildingMap.hpp"
#include "../Simulation/Agent.hpp"
#include <vector>
#include <string>

namespace Futura {

enum class SeverityLevel {
    INFO,
    WARNING,
    CRITICAL
};

struct FacilityRecommendation {
    SeverityLevel severity;
    std::string location;
    std::string riskDescription;
    std::string recommendationText;
};

class RiskAnalyzer {
public:
    static std::vector<FacilityRecommendation> analyzeFacility(
        const BuildingMap& map,
        const std::vector<Agent>& agents,
        float evacTime
    );
};

} // namespace Futura

#endif // RISK_ANALYZER_HPP
