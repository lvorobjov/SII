/*   �ணࠬ��:  ���������������� �������                    */
/*   ����:       Prog.pro                                    */
/*   �����祭��: ���������� ࠡ��� �ᯥ�⭮�              */
/*              ��⥬�. �� �த�樮���� ��⥬�,          */
/*              ����������� �� �ࠢ����                     */
/*   ����砭��:  �� ��⥬� ��� �����䨪�樨 ��⥣�ਨ     */
/*             ��஦��� ������. ���⥬� �ᯮ���� ������⢮ */
/*             �த�樮���� �ࠢ�� ��� �뢮��               */
/*             �襭��                                       */

     domains
     database
          xpositive (symbol)
          xnegative (symbol)
     predicates
          do_expert
          consulting
          in_out (symbol)
          roadsign (symbol)
          priznak (symbol)
          npriznak (symbol)
          remember (symbol,symbol)
          clear_facts
     goal
          do_expert.
          
     clauses
          /* ���⥬� ���짮��⥫�᪮�� ����䥩� (���) */
     do_expert :-
          makewindow (1,112,6,"���������� �������",1,12,22,58),
          nl,write ("      * * * * * * * * * * * * * * * * * *  * * * * *"),
          nl,write ("      *         ���������������� �������           *"),
          nl,write ("      *         �� ������������� ��������� ������  *"),
          nl,write ("      *                                            *"),
          nl,write ("      *          � � � � � � � � !                 *"),
          nl,write ("      *                                            *"),
          nl,write ("      *   �p� �⢥� �� ���p��� ����p���          *"),
          nl,write ("      *              'yes' ���  'no'.              *"),
          nl,write ("      * * * * * * * * * * * * * * * * * * * * ** * *"),
          nl,nl,
          consulting,
          write ("������ ���� ������� ."),nl,
          readchar (_),
          removewindow,
          exit.
     consulting :- 
          roadsign (X),!,nl,
          write ("        ��㯯� ��஦���� ����� : ",X,"."),nl,
          clear_facts.
     consulting :-
          nl,write ("        �p����, � �� ���� ��� ������ ! "),
          clear_facts.
     in_out (Y) :- 
          write ("      ���p�� :-  ",Y," ?"),
          readln (Reply),
          remember (Y,Reply).
               
          /*         ��������  ������          */
     priznak (Y) :-
          xpositive (Y),!.      
     priznak (Y) :-
          not (xnegative (Y)),
          in_out (Y),
          priznak (Y).
     npriznak (Y) :-
          xnegative (Y),!.
     npriznak (Y) :-
          not (xpositive (Y)),
          in_out (Y),
          npriznak (Y).
     remember (Y,yes) :-
          asserta (xpositive (Y)).
     remember (Y,no) :-
          asserta (xnegative (Y)).
     clear_facts :-
          retract (xnegative (_)),
          fail.
     clear_facts :-
          retract (xpositive (_)),
          fail.
                                         
          /*     �������������  �������        */
          
	roadsign("�।�०���騥 �����") :-
		priznak("��ଠ = ��㣮�쭨�, ���設� �����"),!.
	roadsign("����� �ਮ���") :-
		priznak("��ଠ = ��㣮�쭨�, ���設� ����"),!.
	roadsign("����� �ਮ���") :-
		priznak("��ଠ = ���쬨㣮�쭨�"),!.
	roadsign("����� �ਮ���") :-
		priznak("��ଠ = ����"),!.
	roadsign("���ଠ樮���-㪠��⥫�� �����") :-
		priznak("��ଠ = �����"),
		npriznak("����稥 ��᭮� ��५��").
	roadsign("����� �ਮ���") :-
		priznak("��ଠ = �����"),
		priznak("����稥 ��᭮� ��५��"),!.
	roadsign("�।�०���騥 �����") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = ����"),
		priznak("��ଠ ����७���� ����� = ��� �����"),
		priznak("��ࠪ�� ⥪�� = ���������"),
		npriznak("����稥 �࠭ᯮ��"),
		priznak("��� ������ = ����������"),!.
	roadsign("���ଠ樮���-㪠��⥫�� �����") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = ����"),
		priznak("��ଠ ����७���� ����� = ��� �����"),
		priznak("��ࠪ�� ⥪�� = ���������"),
		npriznak("����稥 �࠭ᯮ��"),
		priznak("��� ������ = ��த᪨�"),!.
	roadsign("�����窨 � ��஦�� ������") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = ����"),
		priznak("��ଠ ����७���� ����� = ��� �����"),
		priznak("��ࠪ�� ⥪�� = ���������"),
		priznak("����稥 �࠭ᯮ��"),!.
	roadsign("�����窨 � ��஦�� ������") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = ����"),
		priznak("��ଠ ����७���� ����� = ��� �����"),
		priznak("��ࠪ�� ⥪�� = ��᫠, ������� ����७��"),!.
	roadsign("�����窨 � ��஦�� ������") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = ����"),
		priznak("��ଠ ����७���� ����� = ��� �����"),
		priznak("��ࠪ�� ⥪�� = ���, �६�, ��� ������"),!.
	roadsign("���ଠ樮���-㪠��⥫�� �����") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = ����"),
		priznak("��ଠ ����७���� ����� = ��� �����"),
		priznak("��ࠪ�� ⥪�� = ������᪨� ��������"),!.
	roadsign("���ଠ樮���-㪠��⥫�� �����") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = ����"),
		priznak("��ଠ ����७���� ����� = ��� �����"),
		priznak("��ࠪ�� ⥪�� = ����� ����"),!.
	roadsign("������騥 �����") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = ����"),
		priznak("��ଠ ����७���� ����� = ���"),
		npriznak("����稥 ��५��"),!.
	roadsign("�।���뢠�騥 �����") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = ����"),
		priznak("��ଠ ����७���� ����� = ���"),
		priznak("����稥 ��५��"),!.
	roadsign("���ଠ樮���-㪠��⥫�� �����") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = ����"),
		priznak("��ଠ ����७���� ����� = ������"),!.
	roadsign("���ଠ樮���-㪠��⥫�� �����") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = �����"),!.
	roadsign("����� �ࢨ�") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = �����"),
		npriznak("����稥 �࠭ᯮ��"),
		priznak("��� ������ = ����������"),!.
	roadsign("���ଠ樮���-㪠��⥫�� �����") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = �����"),
		npriznak("����稥 �࠭ᯮ��"),
		priznak("��� ������ = ��த᪨�"),!.
	roadsign("����� �ࢨ�") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = �����"),
		npriznak("����稥 �࠭ᯮ��"),
		priznak("��� ������ = ����த��"),!.
	roadsign("���ଠ樮���-㪠��⥫�� �����") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = �����"),
		priznak("����稥 �࠭ᯮ��"),!.
	roadsign("�।�०���騥 �����") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = ����"),!.
	roadsign("���ଠ樮���-㪠��⥫�� �����") :-
		priznak("��ଠ = ��אַ㣮�쭨�"),
		priznak("���� 䮭� = ������"),!.
	roadsign("�।�०���騥 �����") :-
		priznak("��ଠ = �㪢� �"),!.
	roadsign("������騥 �����") :-
		priznak("��ଠ = ���"),
		priznak("���� 䮭� = ����"),
		npriznak("����稥 ��᭮� ��५��"),!.
	roadsign("����� �ਮ���") :-
		priznak("��ଠ = ���"),
		priznak("���� 䮭� = ����"),
		priznak("����稥 ��᭮� ��५��"),!.
	roadsign("�।���뢠�騥 �����") :-
		priznak("��ଠ = ���"),
		priznak("���� 䮭� = �����"),
		npriznak("����稥 ����⮢��"),!.
	roadsign("������騥 �����") :-
		priznak("��ଠ = ���"),
		priznak("���� 䮭� = �����"),
		priznak("����稥 ����⮢��"),!.
	roadsign("������騥 �����") :-
		priznak("��ଠ = ���"),
		priznak("���� 䮭� = ����"),!.
		