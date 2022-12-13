@extends('layouts.app')

@section('content')

<style>

red{
    color:red;
}


</style>

<div class="container">
    <div class="row justify-content-center">
        <div class="col-md-8">
            <div class="card">
                <div class="card-header">{{ __('Cadastro de equipes') }}</div>

                <div class="card-body">

                    @if ($team->id)
                        <form id="form" action="{{route('update',$team)}}" method="post">
                            @csrf
                    @else
                        <form id="form" action="{{route('save')}}" method="post">
                            @csrf
                    @endif

                        <div class='row'>
                            <label class='col-md'>Nome da equipe <red>*</red>
                                <input name="team_name" class="form-control @error('team_name') is-invalid @enderror" type="text" value="{{$team->team_name}}">
                                
                                @error('team_name')
                                    <div class="alert alert-danger">{{ $message }}</div>
                                @enderror
                            </label>

                            <label class='col-md-3'>Tempo
                                <input name="time" class="form-control" type="time" max="18:00" min="00:00" value="{{$team->time}}">
                                <span class="form-text text-muted">Tempo que a equipe levou para explodir ou desarmar.</span>
                            </label>

                            <label class='col-md-3'>Dificuldade
                                <select name="difficult" class="form-control">
                                    <option value="3" @if ($team->difficult == 3) {{"selected"}} @endif>Normal</option>
                                    <option value="2" @if ($team->difficult == 2) {{"selected"}} @endif>Fácil</option>
                                    <option value="1" @if ($team->difficult == 1) {{"selected"}} @endif>Muito fácil</option>
                                </select>
                            </label>

                        </div>

                        <div class='row'>

                            <label class='col-md-3'>
                                <input type="hidden" name="exploded" value="0" >
                                <input name="exploded" class="form-check-input" type="checkbox" @if ($team->exploded == 1) {{"checked"}} @endif value="1">
                                A bomba explodiu
                            </label>
                            
                            
                        </div>

                        @for ($i = 0; $i < 5; $i++)
                        <div>
                            <h5 class="border-bottom mt-5">Membro {{$i+1}}</h5>
                            <div class='row'>

                                <label class='col-md-6'> Nome <red>*</red>
                                    <input name="name[{{$i}}]" class="form-control" type="text" value="{{$members[$i]->name ?? null}}">
                                </label>

                                <label class='col-md-2'> Idade
                                    <input name="age[{{$i}}]" class="form-control" type="text" value="{{$members[$i]->age ?? null}}">
                                </label>

                                <label class='col-md-4'> Curso
                                    <input name="course[{{$i}}]" class="form-control" type="text" value="{{$members[$i]->course ?? null}}">
                                </label>

                                {{--<label class='col-md-2'> Período
                                    <input name="course_period[{{$i}}]" class="form-control" type="text" value="">
                                </label>

                                <label class='col-md-3'> Nota para o projeto
                                    <input name="grade[{{$i}}]" class="form-control" type="text" value="">
                                </label>--}}
                            </div>
                        </div>
                        @endfor

                    </form>

                    <div class="row gap-3 mt-3 d-flex justify-content-center">
                        <button type="submit" class="btn btn-primary col-md-2" form="form">Salvar</button>
                        <a href="{{route("new")}}" class="btn btn-secondary col-md-2">Novo</a>
                        <a href="{{route("home")}}" class="btn btn-secondary col-md-2">Rank</a>
                        @if ($team->id)
                            <a href="{{route("foto",$team)}}" class="btn btn-success col-md-2">Foto</a>
                            @if ($canDelete)
                                <a href="{{route("delete",$team)}}" class="btn btn-danger col-md-2">Deletar</a>                            
                            @endif
                        @endif
                    </div>


                </div>
            </div>
        </div>
    </div>
</div>
@endsection
