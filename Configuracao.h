#pragma once

class Configuracao {
private:
    int pergunta  = 3;
    int resposta  = 3;
    int replica   = 2;
    int treplica  = 2;

public:
    int  get_pergunta_tempo()  const { return pergunta;  }
    void set_pergunta_tempo(int t)   { pergunta  = t;    }
    int  get_resposta_tempo()  const { return resposta;  }
    void set_resposta_tempo(int t)   { resposta  = t;    }
    int  get_replica_tempo()   const { return replica;   }
    void set_replica_tempo(int t)    { replica   = t;    }
    int  get_treplica_tempo()  const { return treplica;  }
    void set_treplica_tempo(int t)   { treplica  = t;    }
};
