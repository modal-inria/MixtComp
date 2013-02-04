% \iffalse -*-tex-*-

% $Id: ens-ustl.cls,v 1.7 2006/01/02 16:30:01 mickey Exp $

% Des petits trucs pour presenter proprement les enonces de Cours-TD,
% TD et TP, a l'USTL. La premiere version a ete ecrite le 23 oct 1996.

% Pour l'instant je ne mets que le code, je passerai le tout en
% docstrip quand j'aurais le temps.

%%% modif J. Carle
% modif : \maketitle pour permettre plusieurs documents en un seul fichier (TD puis TP par exemple)
% modif : Options par d�faut pour logo = "iutinfo"
% ajout de doc : liste des options/commandes/environements de cette classe
% ajout : commande "rep"
% ajout : option de classe "name/noname" pour ajouter une ligne nom/prenom/groupe pour DS
% ajout : affichage dur�e estim�
% ajout option couleur de la correction (variable interne), green par d�faut
% modif : affichage "CORRECTION" en ent�te de page si necessaire
% modif : footnote avec symbole plut�t qu'avec num�ro

% -- liste des options :
% cadre (defaut) / nocadre
% correction (defaut) / nocorrection
% iutinfo
% name / noname (default)

% -- liste des commandes :
%\etablissement{}
%\formation{}
%\enseignant{}
%\matiere{}
%\titre{}
%\duration{}
%
%\exercice{}
%\exercicen{}{}
%\question{}
%\sousquestion{}
%\bareme{}
%
%\correc{}
%\rep{}{}
%
%\fichiersource{}
%\touche{}
%\email{}
%\commande{}

% -- listes environnements
% avertissement
% correction

% \fi

\def\fileversion{v0.5}
\def\filedate{2004/04/05}
\def\docdate{}

% petite variable bien sympa
\def\@colorCorrections{red}

\NeedsTeXFormat{LaTeX2e}

%\ProvidesClass{ens-ustl}[\filedate%
%\space LaTeX2e document class%
%\space ==========(\fileversion\space B. BEAUFILS, LIFL, modified by J. CARLE)==========]
\ProvidesClass{ens-ustl}[\filedate\space\fileversion]

% Quelques variables conditionnelles et leurs valeurs par defaut
\newif\ifcadre
\cadrefalse

\newif\ifcorrections
\correctionsfalse

\newif\ifname
\namefalse

\newif\ifiutinfo
\iutinfofalse
\newif\iflogo
\logofalse
%\newif\ifeudil
%\eudilfalse
%\newif\ifufrieea
%\ufrieeafalse

\DeclareOption{corrections}{\correctionstrue}
\DeclareOption{nocorrections}{\correctionsfalse}
\DeclareOption{cadre}{\cadretrue}
\DeclareOption{nocadre}{\cadrefalse}
\DeclareOption{name}{\nametrue}
\DeclareOption{noname}{\namefalse}
\DeclareOption{iutinfo}{\iutinfotrue}%\iuteudilfalse\ufrieeafalse}
\DeclareOption{logo}{\logotrue}
\DeclareOption{nologo}{\logofalse}
%\DeclareOption{eudil}{\eudiltrue\iutinfofalse\ufrieeafalse}
%\DeclareOption{ufrieea}{\ufrieeatrue\iutinfofalse\eudilfalse}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}

\ProcessOptions

%\ifcorrections
%\ClassWarningNoLine{ens-ustl}{%
%    *************** AVEC LES CORRECTIONS **************}
%\else
%\ClassWarningNoLine{ens-ustl}{%
%    Sans les corrections}
%\fi

\ifiutinfo
\newcommand{\@logo}{iutaustl}
\else
\logofalse
\fi

\LoadClass{article}
\RequirePackage[utf8]{inputenc}
\RequirePackage[T1]{fontenc}
\RequirePackage[frenchb]{babel}
\RequirePackage{color}
\RequirePackage{geometry}
\RequirePackage{graphicx}
%\RequirePackage{epsfig}
\RequirePackage{ifthen}
\RequirePackage{latexsym}
\RequirePackage{moreverb}
\RequirePackage{verbatim}
\RequirePackage{eurosym}
\RequirePackage{amsmath}
\RequirePackage{amsfonts}
\RequirePackage{amssymb}
%\RequirePackage{yhmath}
\RequirePackage{mathrsfs}
\RequirePackage{eucal}
\RequirePackage{ulem}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\geometry{paper=a4paper,%
  left=12.5mm,%
  right=12.5mm,%
  top=5mm,%
  bottom=20mm,%
  foot=4mm}

% Symboles plut�t que num�ros pour les notes de bas de page
\def\thefootnote{\fnsymbol{footnote}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Tout d'abord je cree les variables utilisees par \maketitle avec leur valeur
% par defaut ...
\newcommand{\@etablissement}{\empty}
\newcommand{\@chapitre}{\empty}
\newcommand{\@formation}{\empty}
\newcommand{\@enseignant}{\empty}
\newcommand{\@matiere}{\empty}
\newcommand{\@titre}{\empty}
\newcommand{\@duration}{\empty}
\newcommand{\@notetotal}{\empty}
\newcommand{\parboxdot}[1]{\parbox{#1}{~\vskip 1mm \dotfill}}
% ... ensuite je cree les commandes qui permettent a l'utilisateur de modifier
% le contenu de ces variables ...
\newcommand{\etablissement}[1]{\renewcommand{\@etablissement}{\mbox{\sffamily{#1}}}}
\newcommand{\chapitre}[1]{\renewcommand{\@chapitre}{\sffamily{#1}}}
\newcommand{\formation}[1]{\renewcommand{\@formation}{\sffamily{#1}}}
\newcommand{\enseignant}[1]{\renewcommand{\@enseignant}{\sffamily{#1}}}
\newcommand{\matiere}[1]{\renewcommand{\@matiere}{\sffamily{#1}}}
\newcommand{\duration}[1]{\renewcommand{\@duration}{\sffamily{#1}}}
\newcommand{\notetotal}[1]{\renewcommand{\@notetotal}{#1}}
% ... pour le titre on fait un tableau histoire de pouvoir utiliser le retour
% a la ligne ...
\newcommand{\titre}[1]{%
  \renewcommand{\@titre}{\bfseries\begin{tabular}{c}#1\end{tabular}}}
% Je supprime les fonctions de preambule classiques que je n'utilise pas.
\global\let\author\relax
\global\let\@author\@empty
\global\let\title\relax
\global\let\@title\@empty
%% redefinition de la valeur par defaut de la date a rien
\renewcommand{\@date}{~}

% Je redefini la valeur par defaut de la date, pour ne laisser apparaitre que
% l'annee universitaire en cours, en considerant l'annee universitaire allant
% de septembre (annee-1) a juin (annee).
%% \newcounter{@avant}
%% \setcounter{@avant}{\year}
%% \newcounter{@apres}
%% \setcounter{@apres}{\year}
%% \renewcommand{\@date}{%
%%   \ifthenelse{\month < 7}%
%%   {\addtocounter{@avant}{-1}}
%%   {\addtocounter{@apres}{1}}
%%   \the@avant--\the@apres}

% Calcul de la largeur de la partie gauche du titre
\newlength\@lggauche
\setlength\@lggauche{.2\textwidth}
%\newlength\@lgetab
%\newlength\@lglogo
%\settowidth\@lglogo{\includegraphics[height=3\baselineskip]{\@logo}}
%\settowidth\@lgetab{\footnotesize\textsf{\@etablissement}}
%%min(lggauche, max(lglogo, lgetab))
%\ifnum\@lglogo >\@lgetab
%  \ifnum\@lggauche <\@lglogo
%    \setlength\@lggauche{\@lglogo}
%  \fi
%\else
%  \ifnum\@lggauche >\@lgetab
%    \setlength\@lggauche\@lgetab
%  \fi
%\fi

% Je redefini maintenant la commande \maketitle.
\renewcommand{\maketitle}{
% Je ne veux pas qu'il y ait de figure ou de table avant le titre qui doit
% donc etre place en haut de la page ...
  \suppressfloats[t]
  \begin{center}
    \begin{minipage}{.25\textwidth}
      \iflogo
      \centering
      \includegraphics[height=3\baselineskip]{\@logo}
      \else~
      \fi
    \end{minipage}%
    \hfill
    \begin{minipage}{.5\textwidth}
      \centering
                                %       \ifx\@matiere\empty\else%
%         {\large\textmd{\textsf{\@matiere}}}
%         \vspace{1em}\vfil
%       \fi
      \ifcadre{%
        \framebox{~\large\textsf{\@titre}~}%
      }
      \else{%
        {~\large\textsf{\@titre}~}%
      }
      \fi
    \end{minipage}%
    \hfill
    \begin{minipage}{.25\textwidth}
      \raggedleft\small
      \@etablissement
      \par
      \@matiere
      \par
      \@formation
      \par
      \@enseignant
      \par
      \@date
    \end{minipage}%
  \end{center}
% ... et on finit par un trait pour separer le titre de l'enonce.
  \hrule%
  \medskip

% et enfin, emplacement pour ecrire ses nom, prenom et groupe
  \if\@duration\empty
  \ifcorrections
  ~ \hfill {\color{\@colorCorrections}\bfseries CORRECTION} \hfill\ %
  \medskip
  \hrule
  \fi
  \else
  \medskip

  \ifcorrections
  Temps estim� : \@duration\hfill {\color{\@colorCorrections}\bfseries CORRECTION} \hfill\ %
  \else
  Temps estim� : \@duration%
  \fi
  \smallskip
  \hrule\smallskip%
  \fi
        \ifname
                \ifcorrections
                %\ClassWarningNoLine{ens-ustl}{********* NOM INUTILE AVEC LES CORRECTIONS **********}
                \else
                        \smallskip\noindent
                        {\sc Nom} : \dotfill\hskip 5mm
                        {\sc Pr�nom:} \dotfill\hskip 5mm
                        \ifiutinfo
                        \parbox{25mm}{\sc Groupe : \dotfill}
                        \else
                        \parbox{25mm}{\sc Classe : \dotfill}
                        \fi
                        \if\@notetotal\empty
                        \else
                        \hskip 5mm
                        \parbox{25mm}{\sc Note : \dotfill /\@notetotal}
                        \fi
                \smallskip
                \hrule\smallskip%
          \fi
        \else
                \empty
        \fi
% Il faut placer les eventuelles notes de bas de page.
        \smallskip%
  \@thanks

% Mise � zero du compteur d'exercice
\setcounter{exercice}{0}

%modification du style de itemize
\renewcommand{\labelitemi}{$\bullet$}

% C'est fini je peux vider les variables utilisees seulement
% sur le titre de la premi�re page.
% On garde les autres pour les prochainnes page avec ent�te
\global\let\thanks\relax
\global\let\@thanks\@empty
%\setcounter{@avant}{0}
%\setcounter{@apres}{0}
%\global\let\maketitle\relax
%\global\let\@maketitle\relax
%\global\let\date\relax
%\global\let\@date\@empty
%\global\let\and\relax
%\global\let\@etablissement\@empty
%\global\let\etablissement\relax
%\global\let\@formation\@empty
%\global\let\formation\relax
%\global\let\@enseignant\@empty
%\global\let\enseignant\relax
%\global\let\@matiere\@empty
%\global\let\matiere\relax
%\global\let\@titre\@empty
%\global\let\titre\relax
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newenvironment{avertissement}[1][\empty]{%
  \bfseries\noindent\parindent=0cm
  \vfil
  \ifx#1\empty\hrule%
  \else
  \hrulefil~#1~\hrulefil
  \fi
  \medskip}
{%
  \smallskip
  \hrule\vfil}

\newenvironment{forceleft}{%
  \begin{flushleft}
    \begin{minipage}[h]{4cm}
}%
{%
\end{minipage}
\end{flushleft}
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% redefintion de \section ....
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\renewcommand \thepart {\@Roman\c@part}
\renewcommand \thesection {\@Roman\c@section}
\renewcommand\thesubsection   {\@arabic\c@subsection}
\renewcommand\thesubsubsection{\@alph\c@subsubsection}
\renewcommand\theparagraph    {\thesubsubsection.\@arabic\c@paragraph}
\renewcommand\thesubparagraph {\theparagraph.\@arabic\c@subparagraph}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Je cree de nouveaux compteurs pour les numeros d'exercices, de
% questions, et sous-questions.
\newcounter{exercice}[section]
\newcounter{question}[exercice]
\newcounter{sousquestion}[question]

% Je cree de nouvelles commandes utilisables comme les classiques \section{}
% pour les entetes d'exercices, de questions, et de sous-questions.
\newcommand{\exercice}[2][Exercice]{\stepcounter{exercice}%
    \subsubsection*{\underline{#1 \ifnum\c@section>\z@{\thesection
          .}\fi\theexercice}~:~#2}}
\newcommand{\exercicen}[2][Exercice]{%
    \subsubsection*{\underline{#1}~:~#2}}

\newcommand{\question}[1][\empty]{\stepcounter{question}%
  {\par\noindent\textbf{Q \thequestion \ifx#1\empty.\else~#1.\fi}}}
\newcommand{\sousquestion}[1][\empty]{\stepcounter{sousquestion}%
  {\par\noindent\textbf{Q \thequestion .\thesousquestion
      \ifx#1\empty.\else~#1.\fi}}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Une petite commande qui permet de donner le bareme par exercice
\newcommand{\bareme}[1]{\hfill{\small\textmd{\textsl{(#1~points)}}}}
\newcommand{\baremesin}[1]{\hfill{\small\textmd{\textsl{(#1~point)}}}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% L'environnement correction est defini pour inclure ou non des
% parties du textes. Si l'inclusion doit etre faite il faut que le
% test \ifcorrection soit vrai, par exemple par l'option correction.
% Quand on ne veut pas les corrections on utilise l'environnement
% comment du package verbatim.
\ifcorrections
%\definecolor{light}{gray}{.5}
\newenvironment{correction}{\par
  \color{\@colorCorrections}}{\par}
\newcommand{\correc}[1]{{\color{\@colorCorrections} #1}}
\else
\newcommand{\correc}[1]{\empty}
\let\correction=\comment
\let\endcorrection=\endcomment
\fi

%% environnement propri
%% \newenvironment{propri[1]}%
%% {\begin{center}%
%%   \textbf{#1}%
%%   \framebox{%
%%   \begin{minipage}{15cm}}%
%% }%
%% {\end{minipage}}%
%% \end{center}}



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% commande rep pour �crire un texte ou une correction � la m�me
% position dans le texte (l'un remplace l'autre)
% exemple : \rep{correction}{nocorrection}
\newcommand\rep[2]{\ifcorrections{\color{\@colorCorrections} #1}\else{#2}\fi}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Inclusion de fichiers sources exterieurs.
\newcommand{\fichiersource}[2][\small]{%
    {#1\listinginput{1}{#2}}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newcommand{\touche}[1]{\fbox{\texttt{#1}}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newcommand{\email}[1]{\texttt{#1}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newcommand{\commande}[1]{\texttt{#1}}

\newsavebox{\fmbox}
\newenvironment{encadre}[1]
     {\begin{lrbox}{\fmbox}\begin{minipage}{#1}}
     {\end{minipage}\end{lrbox}\bigskip\noindent\fbox{\usebox{\fmbox}}\bigskip}

\pagestyle{empty}
\renewcommand{\labelitemi}{$\bullet$}
\newcommand{\ligne}{\bigskip

\dotfill}
% Fin.
