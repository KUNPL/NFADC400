//////////////////////////////////////
//                                  //
//   rootlogon for NFADC400 Data    //
//                                  //
//  Author: Genie Jhang             //
// Contact: geniejhang@majimak.com  //
//    Date: 2013. 11. 06            //
//                                  //
//////////////////////////////////////

void rootlogon()
{
  cout << endl;
  cout << " ///////////////////////////////////////////////" << endl;
  cout << " //                                           //" << endl;
  cout << " // Plotting script is loaded!                //" << endl;
  cout << " //                                           //" << endl;
  cout << " //   Usage)                                  //" << endl;
  cout << " //     load(\"DATAFILE.root\");                //" << endl;
  cout << " //     plot(eventNum, moduleNum, channelNum) //" << endl;
  cout << " //                                           //" << endl;
  cout << " //   Note)                                   //" << endl;
  cout << " //     If you want to analyze data in other  //" << endl;
  cout << " //     folder, you should copy files below.  //" << endl;
  cout << " //                                           //" << endl;
  cout << " //   File List)                              //" << endl;
  cout << " //     NFADC400Constant.hh                   //" << endl;
  cout << " //     libNFADC400.so                        //" << endl;
  cout << " //     rootlogon.C                           //" << endl;
  cout << " //     rootalias.C                           //" << endl;
  cout << " //                                           //" << endl;
  cout << " ///////////////////////////////////////////////" << endl;
  cout << endl;

  if (gSystem -> AccessPathName("libNFADC400.so")) {
    cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << " !!                                          !!" << endl;
    cout << " !! Type 'make' to build the shared library! !!" << endl;
    cout << " !!                                          !!" << endl;
    cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << endl;
  }
}
