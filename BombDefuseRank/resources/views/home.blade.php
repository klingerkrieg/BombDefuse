@extends('layouts.app')

@section('title', 'Início - Defuse The Bomb')

@section('content')

<main id="guest">
    <div class="container">
        <img class="logo" src="{{ asset("img/logo.png") }}" alt="Logo">
        <h2></h2>
        <h1 class="fw-bold text-center">Bem-vindo ao Defuse The Bomb</h1>
        <div class="button_home_container">
            <a class="btn_home" href="{{ route('rank') }}">
                <i class="bi bi-trophy"></i> 
                Rank
            </a>
            <a class="btn_home" href="{{ route('fila') }}">
                <i class="bi bi-list-ol"></i>
                Fila
            </a>

            @if(Auth::check())
                <a class="btn_home" href="{{ route('dashboard') }}"><i class="bi bi-bar-chart"></i> Dashboard</a>
            @else
                <a class="btn_home outline" href="{{ route('login') }}">
                    <svg xmlns="http://www.w3.org/2000/svg" height="32px" viewBox="0 -960 960 960" width="32px" fill="currentColor"><path d="M480.67-120v-66.67h292.66v-586.66H480.67V-840h292.66q27 0 46.84 19.83Q840-800.33 840-773.33v586.66q0 27-19.83 46.84Q800.33-120 773.33-120H480.67Zm-63.34-176.67-47-48 102-102H120v-66.66h351l-102-102 47-48 184 184-182.67 182.66Z"/></svg>
                    Login
                </a>
            @endif
        </div>
        <div>
            <p class="text-center" style="font-size: 1.3em;">Visite nossa página no Instagram!</p>
            <div class="d-flex justify-content-center align-items-center">
                <a href="https://www.instagram.com/defuse_the_bomb.nc/" target="_BLANK">
                     <img style="width:44px;height:44px;" src="{{asset('img/insta.webp')}}" />
                </a>
            </div>
        </div>
    </div>
</main>

@endsection