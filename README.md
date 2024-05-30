# Markov Chain Text Mixer
## Funzionamento


Questo programma esegue due compiti distinti :

 - *COMPITO 1* :  dato un testo in Italiano, produca una tabella contenente per ogni parola del testo le 
parole immediatamente successive e la loro frequenza di occorrenza nel testo;
 - *COMPITO 2* : generi un testo in maniera casuale usando la frequenza delle parole calcolata nel 
punto 1.

---



## Utilizzo

Dentro la directory sorgente, è contenuta la directory 'DoubleVersion', contenente l'eseguibile del programma

Dentro tali directory, sono contenuti i rispettivi eseguibili : 

DoubleVersion/MCtextMixer

Si apra la linea di comando, spostandosi nella cartella in cui è presente il file eseguibile


---


### Comandi 

compito 1 : ```./MCtextMixer-1 -i [input file] -o [output file]  -m```

compito 2 : ``` ./MCtextMixer-2 -o [output file] -i [input file] -n [numero parole] -s[parola iniziale]  -m```

Tutti i comandi sono obbligatori, escluso il comando ``` -s ``` ed il comando ``` -m ```.

Esempio di utilizzo del programma :

```./MCtextMixer-1 -i testFile/commedia.txt -o OUTPUT/CSV/commedia.csv  ```

``` ./MCtextMixer-2 -o OUTPUT/randomTxt/commedia.txt  -i OUTPUT/CSV/commedia.csv -n 100000 ```

```./MCtextMixer-1 -i testFile/commedia.txt -o OUTPUT/CSV/commedia.csv  -m```

``` ./MCtextMixer-2 -o OUTPUT/randomTxt/commedia.txt  -i OUTPUT/CSV/commedia.csv -n 100000 -s selva```

---

Spiegazione dettagliata : 

``` -1 ``` Tale comando specifica che si vuole eseguire il compito 1 del programma, ossia la conversione da file di testo, a file CSV contenente le frequenze. Tale comando non può essere usato in combinazione con il comando successivo.

``` -2 ```  Tale comando specifica che si vuole eseguire il compito 2 del programma, ossia la 
generazione casuale del testo a partire da un file CSV contenete le frequenze. Tale comando non può essere usato in combinazione con il comando precedente.

``` -i ``` Specifica il file di input che dovrà essere selezionato dal programma, se si vuole eseguire il compito 2, tale file dovrà essere un file CSV, se invece si vuole eseguire il compito 1, il file dovrebbe rappresentare un semplice file di testo.

``` -o ``` Specifica il file di output che dovrà essere generato dal programma, se si vuole eseguire il compito 2, tale file sarà essere un file di testo, se invece si vuole eseguire il compito 1, tale file sarà un file CSV. Se viene selezionato un file inesistente, verrà creato, se viene selezionato un file esistente, verrà sovrascritto. Non è possibile selezionare un percorso inesistente, la directory in cui si vuole generare il file deve esistere.

``` -n ``` Specifica il numero di parole che verranno generate nel compito 2. 

``` -s ``` Specifica la parola che verrà utilizzata come punto di partenza nel compito 2, 
è necessario che la parola sia presente nel file CSV. La parola specificata deve essere 
necessariamente minuscola. Se tale comando è assente, verrà selezionato una punteggiatura casualmente fra quelle presenti, e la prima parola del testo verrà scelta casualmente fra quelle che seguono tale punteggiatura.

``` -m ``` Se presente, verrà eseguita la versione multi processo del programma, altrimenti, verrà eseguita la versione con un singolo processo.

