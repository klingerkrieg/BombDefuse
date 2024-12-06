<?php

namespace App\Http\Controllers;

use App\Models\Member;
use App\Models\Team;
use App\Models\TeamQueue;
use Illuminate\Http\Request;

class HomeController extends Controller
{

    public function index() {
        return view('home');
    }

    public function fila() {

        // Legenda
        // 0 - Equipe não participou
        // 1 - Equipe participou

        $mensagem = "Olá, somos da equipe Defuse The Bomb. Sua equipe será a próxima a participar! Pedimos para que reúna os demais membros da equipe e se possível tentem chegar aqui na sala dentro de 5 minutos, pois há outras equipes na fila de espera.";
        $mensagemCodificada = urlencode($mensagem);

        $equipes = TeamQueue::where('status', '!=', 1)
                    ->orderBy('created_at', 'asc')
                    ->get();

        return view('fila', [
            'equipes'=> $equipes, 
            'mensagemCodificada'=> $mensagemCodificada
        ]);
    }
    
    public function rank() {
        #$defuseds = Team::with('members')->orderBy('time','asc')->where("exploded",0)->get();
        #$explodeds = Team::with('members')->orderBy('time','desc')->where("exploded",1)->get();
        
        #$teams = array_merge($defuseds->toArray(),$explodeds->toArray());
        $teams = Team::with('members')
        ->where("event_name", env("APP_EVENT_NAME"))
        ->whereNotNull("time")
        ->get()
        ->sortByDesc("score");
        
        return view('rank', ["teams"=>$teams]);
    }

}
