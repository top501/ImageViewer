/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    BiasCorrector.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <string>
#include <vector>
#include <vnl/vnl_math.h>


#include "mydefs.h"
#include "imageutils.h"
#include "OptionList.h"
#include "itkMRIBiasFieldCorrectionFilter.h"


void print_usage()
{
  print_line("MRIBiasCorrection 1.0 (21.June.2001)");

  print_line("usage: BiasCorrector --input file" ) ;
  print_line("       --output file") ;
  print_line("       --class-mean mean(1) ... mean(i)" ) ;
  print_line("       --class-sigma sigma(1) ... sigma(i)" ) ;
  print_line("       --use-log [yes|no]") ;
  print_line("       [--input-mask file]" ) ;
  print_line("       [--output-mask file]" ) ;
  print_line("       [--degree int] ") ;
  print_line("       [--growth double] [--shrink double] [--max-iteration int]");
  print_line("       [--init-step-size double] ");
  print_line("       [--use-slab-identification [yes|no]]") ;
  print_line("       [--slice-direction [0-2]]" ) ;

  print_line("");

  print_line("--input file") ;
  print_line("        input image file name [meta image format]" );
  print_line("--output file") ;
  print_line("        output image file name [meta image format]" );
  print_line("--class-mean mean(1),...,mean(i)" ) ;
  print_line("        intensity means  of the differen i tissue classes") ;
  print_line("--class-sigma sig(1),...,sig(i)" ) ; 
  print_line("        intensity sigmas of the different i tissue clases") ;
  print_line("        NOTE: The sigmas should be listed in the SAME ORDER") ;
  print_line("              of means");
  print_line("        and each value should be SEPERATED BY A SPACE)") ;
  print_line("--input-mask file" ) ;
  print_line("        mask input with file [meta image format]");
  print_line("--output-mask file" ) ;
  print_line("        mask output with file [meta image format]");
  print_line("--degree int") ;
  print_line("        degree of legendre polynomial used for the") ;
  print_line("        approximation of the bias field" );
  print_line("--use-log [yes|no]") ;
  print_line("        if yes, assume a multiplicative bias field") ;
  print_line("        (use log of image, class-mean, class-sigma,") ;
  print_line("         and init-step-size)" );
  print_line("--growth double") ;
  print_line("        stepsize growth factor. must be greater than 1.0") ;
  print_line("        [default 1.05]" ) ;
  print_line("--shrink double") ;
  print_line("        stepsize shrink factor ") ;
  print_line("        [default growth^(-0.25)]" ) ; 
  print_line("--max-iteration int") ;
  print_line("        maximal number of iterations") ;
  print_line("        [default 20]" ) ;
  print_line("--init-step-size double") ;
  print_line("        inital step size [default 1.02]" );
  print_line("--use-slab-identification [yes|no]") ;
  print_line("       if yes, the bias correction will first identify slabs,") ;
  print_line("       and then apply the bias correction to each slab") ;
  print_line("--slice-direction [0-2]" ) ;
  print_line("        slice creation direction ( 0 - x axis, 1 - y axis") ;
  print_line("        2 - z axis) [default 2]") ;

  print_line("");

  print_line("example: BiasCorrector --input sample.mhd") ;
  print_line("         --output sample.corrected.mhd") ;
  print_line("         --class-mean 1500 570") ;
  print_line("         --class-sigma 100 70 --use-log yes");
  print_line("         --input-mask sample.mask.mhd ") ;
  print_line("         --output-mask sample.mask2.mhd ") ;
  print_line("         --degree 3 --growth 1.05 --shrink 0.9");
  print_line("         --max-iteration 2000 --init-step-size 1.1") ;
  print_line("         --use-slab-identification no") ;
  print_line("         --slice-direction 2") ;
}


int main(int argc, char* argv[])
{
  if (argc <= 1)
    {
      print_usage() ;
      exit(0) ;
    }

  OptionList options(argc, argv) ;

  typedef itk::MRIBiasFieldCorrectionFilter<ImageType, ImageType> Corrector ;
  Corrector::Pointer filter = Corrector::New() ;

  std::string inputFileName ;
  std::string outputFileName ;
  std::string inputMaskFileName = "" ;
  std::string outputMaskFileName = "" ;
  bool useLog ;
  int degree ;
  int sliceDirection ;
  vnl_vector<double> coefficientVector ;
  itk::Array<double> classMeans ;
  itk::Array<double> classSigmas ;
  int maximumIteration ; 
  double initialRadius ;
  double growth ;
  double shrink ;

  bool usingSlabIdentification ;

  try
    {
      // get image file options
      options.GetStringOption("input", &inputFileName, true) ;
      options.GetStringOption("output", &outputFileName, true) ;
      options.GetStringOption("input-mask", &inputMaskFileName, false) ;
      options.GetStringOption("output-mask", &outputMaskFileName, false) ;
      
      // get bias field options
      useLog = options.GetBooleanOption("use-log", true, true) ;
      degree = options.GetIntOption("degree", 3, false) ;
      sliceDirection = options.GetIntOption("slice-direction", 2, false) ;
      
      // get energyfunction options
      options.GetMultiDoubleOption("class-mean", &classMeans, true) ;
      options.GetMultiDoubleOption("class-sigma", &classSigmas, true) ;

      // get optimizer options
      maximumIteration = options.GetIntOption("max-iteration", 20, false) ;
      growth = options.GetDoubleOption("growth", 1.05, false) ;
      shrink = pow(growth, -0.25) ;
      shrink = options.GetDoubleOption("shrink", shrink, false) ;
      initialRadius = options.GetDoubleOption("init-step-size", 1.02, false) ;

      // get the filter operation option
      usingSlabIdentification = 
        options.GetBooleanOption("use-slab-identification", false, false) ;
    }
  catch(OptionList::RequiredOptionMissing e)
    {
      std::cout << "Error: The '" << e.OptionTag 
                << "' option is required but missing." 
                << std::endl ;
    }

      
  // load images
  ImagePointer input = ImageType::New() ;
  MaskPointer inputMask = MaskType::New() ;
  MaskPointer outputMask = MaskType::New() ;
  
  try
    {
      std::cout << "Loading images..." << std::endl ;
      loadImage(inputFileName, input) ;
      filter->SetInput(input) ;
      std::cout << "Input image loaded." << std::endl ;
      if (inputMaskFileName != "")
        {
          loadMask(inputMaskFileName, inputMask) ;
          filter->SetInputMask(inputMask) ;
          std::cout << "Input mask image loaded." << std::endl ;
        }
      if (outputMaskFileName != "")
        {
          loadMask(outputMaskFileName, outputMask) ;
          filter->SetOutputMask(outputMask) ;
          std::cout << "Output mask image loaded." << std::endl ;
        }
      std::cout << "Images loaded." << std::endl ;
    }
  catch (ImageIOError e)
    {
      std::cout << "Error: " << e.Operation << " file name:" 
                << e.FileName << std::endl ;
      exit(0) ;
    }

  ImagePointer output = ImageType::New() ;
  output->SetLargestPossibleRegion(input->GetLargestPossibleRegion()) ;
  output->SetRequestedRegion(input->GetLargestPossibleRegion()) ;

  filter->SetOutput(output) ;

  filter->IsBiasFieldMultiplicative(useLog) ;
  filter->SetTissueClassStatistics(classMeans, classSigmas) ;
  filter->SetOptimizerGrowthFactor(growth) ;
  filter->SetOptimizerShrinkFactor(shrink) ;
  filter->SetOptimizerMaximumIteration(maximumIteration) ;
  filter->SetOptimizerInitialRadius(initialRadius) ;
  filter->SetBiasFieldDegree(degree) ;
  filter->SetUsingSlabIdentification(usingSlabIdentification) ;
  filter->SetSlicingDirection(sliceDirection) ;
  filter->Update() ;

  std::cout << "Writing the output image..." << std::endl ;

  try 
    {
      writeImage(outputFileName, output) ;
    }
  catch (ImageIOError e)
    {
      std::cout << "Error: " << e.Operation << " file name:" 
                << e.FileName << std::endl ;
      exit(0) ;
    }
  
  return 0 ;
}
