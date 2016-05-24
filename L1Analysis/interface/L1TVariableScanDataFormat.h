#ifndef VBFHiggsToTauTau_L1Analysis_L1TVariableScanDataFormat
#define VBFHiggsToTauTau_L1Analysis_L1TVariableScanDataFormat

#include <string>
#include <vector>

namespace trgfw{ 

  class L1TVariableScanDataFormat{
  public:
    
    L1TVariableScanDataFormat();
    L1TVariableScanDataFormat(std::string path,std::string name,unsigned nBins,double min,double max);
    ~L1TVariableScanDataFormat();
    
    void   fill         (double   value);
    void   setBinContent(unsigned bin,double value);
    double getBinContent(unsigned bin);
    double getBinLowEdge(unsigned bin);
    
    std::string                getPath();
    std::string                getName();
    int                        getNBins();
    double                     getMin();
    double                     getMax();
    const std::vector<double>* getContent();
    
    void                       scale(double value);
    
    double integral(unsigned binStart,unsigned binEnd);
    void   add(L1TVariableScanDataFormat *input);
    
  private:
    
    // Scan identification
    std::string m_path;
    std::string m_name;
    
    int                 m_nBins;
    double              m_min;
    double              m_max;
    double              m_binSize;
    std::vector<double> m_bins;
    
  };
}

#endif
