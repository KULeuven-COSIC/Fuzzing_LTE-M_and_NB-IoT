This repository contains an implementation of the extended CovFuzz framework for fuzzing LTE-M and NB-IoT, associated with the paper ["Breaking the Silence: Fuzzing LTE-M and NB-IoT Protocols"](link.springer.com/chapter/10.1007/978-3-032-16165-9_31) presented during the MIST workshop 2025.

Note: The original implementation relied on a closed-source protocol stack that cannot be included here due to licensing restrictions. To run this framework, users must integrate their own LTE-M/NB-IoT-compatible protocol stack with the provided interception points.

For the installation instructions please refer to the [original CovFuzz repository](https://github.com/KULeuven-COSIC/CovFuzz_4G_5G_Fuzzing).

To cite this work, please use the following reference:
```bibtex
@InProceedings{10.1007/978-3-032-16165-9_31,
    author="Siro{\v{s}}, Ilja
    and Cavalcanti, Rafael
    and Singel{\'e}e, Dave
    and Preneel, Bart",
    editor="Laborde, Romain
    and Garcia-Alfaro, Joaquin
    and Yazdinejad, Abbas
    and Epiphaniou, Gregory
    and Abie, Habtamu
    and Ranise, Silvio
    and Chora{\'{s}}, Micha{\l}
    and Wo{\'{z}}niak, Micha{\l}
    and Hara, Yuko
    and M{\"u}hlberg, Jan Tobias
    and Greco, Claudia
    and Choo, Kim-Kwang Raymond",
    title="Breaking the Silence: Fuzzing LTE-M and NB-IoT Protocols",
    booktitle="Computer Security. ESORICS 2025 International Workshops",
    year="2026",
    publisher="Springer Nature Switzerland",
    address="Cham",
    pages="524--543",
    abstract="The rise of Internet of Things (IoT) devices has significantly expanded the need for secure communication networks. While security testing for these protocols is crucial, there has been limited research in fuzzing cellular IoT protocol implementations. This paper presents the first framework for fuzz-testing the LTE-M and NB-IoT protocol implementations in IoT devices. Our approach focuses on testing the LTE-M and NB-IoT Attach procedure. It adapts an existing cellular protocol fuzzing framework for the specific requirements of LTE-M and NB-IoT. The framework's effectiveness is validated through testing of four commercially deployed LTE-M/NB-IoT modems, leading to the discovery of 25 new vulnerabilities in two of the modems. This work underscores the importance of security testing emerging IoT technologies and demonstrates the potential of fuzzing to identify vulnerabilities in LTE-M and NB-IoT devices.",
    isbn="978-3-032-16165-9"
}
```